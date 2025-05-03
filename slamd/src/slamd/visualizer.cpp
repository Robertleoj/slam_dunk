#include <flatb/messages_generated.h>
#include <flatb/visualizer_generated.h>
#include <spdlog/spdlog.h>
#include <asio.hpp>
#include <slamd/spawn_window.hpp>
#include <slamd/visualizer.hpp>

namespace slamd {

namespace _vis {

Visualizer::Visualizer(
    std::string name,
    uint16_t port
)
    : port(port),
      name(name) {
    this->client_set = std::make_shared<_net::ClientSet>();

    this->server_thread = std::jthread([this](std::stop_token st) {
        this->server_job(st);
    });
}

void Visualizer::send_tree(
    std::shared_ptr<_tree::Tree> tree
) {
    // find all current geometries
    std::map<_id::GeometryID, std::shared_ptr<_geom::Geometry>>
        current_geometries;

    for (auto& [_, existing_tree] : this->trees) {
        existing_tree->add_all_geometries(current_geometries);
    }

    // get all geometries in tree
    std::map<_id::GeometryID, std::shared_ptr<_geom::Geometry>> tree_geometries;
    tree->add_all_geometries(tree_geometries);

    // get all new geometries
    for (auto& [gid, geom] : tree_geometries) {
        if (current_geometries.find(gid) == current_geometries.end()) {
            this->broadcast(geom->get_add_geometry_message());
        }
    }

    // send tree itself
    auto add_tree_msg = tree->get_add_tree_message();
    this->broadcast(add_tree_msg);
}

void Visualizer::add_scene(
    std::string name,
    std::shared_ptr<Scene> scene
) {
    std::scoped_lock l(this->view_map_mutex);

    if (this->trees.find(scene->id) == this->trees.end()) {
        this->send_tree(scene);
        this->trees.insert({scene->id, scene});
    }

    auto view = _view::View::create(
        name,
        this->shared_from_this(),
        scene,
        slamd::flatb::ViewType_SCENE
    );

    this->view_name_to_view.insert({name, view});
    this->broadcast(view->get_add_view_message());
}

void Visualizer::add_canvas(
    std::string name,
    std::shared_ptr<Canvas> canvas
) {
    std::scoped_lock l(this->view_map_mutex);

    if (this->trees.find(canvas->id) == this->trees.end()) {
        this->send_tree(canvas);
        this->trees.insert({canvas->id, canvas});
    }

    auto view = _view::View::create(
        name,
        this->shared_from_this(),
        canvas,
        slamd::flatb::ViewType_CANVAS
    );

    this->view_name_to_view.insert({name, view});

    this->broadcast(view->get_add_view_message());
}
flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatb::Geometry>>>
Visualizer::get_geometries_fb(
    flatbuffers::FlatBufferBuilder& builder
) {
    std::map<_id::GeometryID, std::shared_ptr<_geom::Geometry>> geom_map;
    for (auto [_, tree] : this->trees) {
        tree->add_all_geometries(geom_map);
    }

    std::vector<flatbuffers::Offset<flatb::Geometry>> geoms;

    for (auto& [_, geom] : geom_map) {
        geoms.push_back(geom->serialize(builder));
    }

    auto geoms_fb = builder.CreateVector(geoms);

    return geoms_fb;
}

void Visualizer::broadcast(
    std::shared_ptr<std::vector<uint8_t>> message_buffer
) {
    this->client_set->broadcast(message_buffer);
}

std::vector<uint8_t> Visualizer::get_state() {
    flatbuffers::FlatBufferBuilder builder(1024);

    auto vis_name_fb = builder.CreateString(this->name);

    std::vector<flatbuffers::Offset<slamd::flatb::Tree>> tree_vec;

    for (auto& [_, tree] : this->trees) {
        auto tree_fb = tree->serialize(builder);
        tree_vec.push_back(tree_fb);
    }

    auto trees_fb = builder.CreateVector(tree_vec);

    std::vector<flatbuffers::Offset<slamd::flatb::View>> view_vec;

    for (auto& [view_name, view] : this->view_name_to_view) {
        auto view_fb = view->serialize(builder);
        view_vec.push_back(view_fb);
    }

    auto views_fb = builder.CreateVector(view_vec);

    auto geometries_fb = this->get_geometries_fb(builder);

    auto state_fb = slamd::flatb::CreateInitialState(
        builder,
        vis_name_fb,
        views_fb,
        trees_fb,
        geometries_fb
    );

    auto message_fb = slamd::flatb::CreateMessage(
        builder,
        slamd::flatb::MessageUnion_initial_state,
        state_fb.Union()
    );

    builder.Finish(message_fb);

    uint8_t* ptr = builder.GetBufferPointer();
    size_t size = builder.GetSize();

    return std::vector<uint8_t>(ptr, ptr + size);
}

void Visualizer::server_job(
    std::stop_token& stop_token
) {
    asio::io_context io;
    asio::ip::tcp::acceptor acceptor(
        io,
        asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)
    );

    std::function<void(void)> accept_loop = [&]() {
        acceptor.async_accept([&](std::error_code ec,
                                  asio::ip::tcp::socket socket) {
            if (!ec) {
                auto conn =
                    std::make_shared<_net::Connection>(std::move(socket));
                conn->write(this->get_state());
                this->client_set->add(conn);
            }
            accept_loop();  // keep accepting
        });
    };

    accept_loop();

    // Spin a thread to cancel io when stop is requested
    std::jthread stop_watcher(
        [&](std::stop_token token) {
            while (!token.stop_requested()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            io.stop();  // tell io to bounce
        },
        stop_token
    );

    io.run();
}

Visualizer::~Visualizer() {
    this->server_thread.request_stop();
}

void Visualizer::hang_forever() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
}  // namespace _vis

VisualizerPtr visualizer(
    std::string name,
    bool spawn,
    uint16_t port
) {
    auto visualizer = std::make_shared<_vis::Visualizer>(name, port);

    if (spawn) {
        slamd::spawn_window(port);
    }

    return visualizer;
}

}  // namespace slamd
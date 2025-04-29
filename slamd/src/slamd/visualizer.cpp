#include <flatb/messages_generated.h>
#include <flatb/visualizer_generated.h>
#include <spdlog/spdlog.h>
#include <asio.hpp>
#include <slamd/visualizer.hpp>

namespace slamd {
namespace _vis {

Visualizer::Visualizer(
    std::string name
)
    : id(_id::VisualizerID::next()),
      name(name) {
    this->client_set = std::make_shared<_net::ClientSet>();

    this->server_thread = std::jthread([this](std::stop_token st) {
        this->server_job(st);
    });
}

void Visualizer::add_scene(
    std::string name,
    std::shared_ptr<Scene> scene
) {
    std::scoped_lock l(this->view_map_mutex);

    if (this->trees.find(scene->id) == this->trees.end()) {
        this->trees.insert({scene->id, scene});
    }

    this->view_name_to_view.insert({name, {scene, slamd::flatb::ViewType_SCENE}}
    );
}

void Visualizer::add_canvas(
    std::string name,
    std::shared_ptr<Canvas> canvas
) {
    std::scoped_lock l(this->view_map_mutex);

    if (this->trees.find(canvas->id) == this->trees.end()) {
        this->trees.insert({canvas->id, canvas});
    }

    this->view_name_to_view.insert(
        {name, {canvas, slamd::flatb::ViewType_CANVAS}}
    );
}

std::vector<uint8_t> Visualizer::get_state() {
    flatbuffers::FlatBufferBuilder builder(1024);

    auto vis_name_fb = builder.CreateString(this->name);

    std::vector<flatbuffers::Offset<slamd::flatb::Tree>> tree_vec;

    for (auto& [_, tree] : this->trees) {
        tree_vec.push_back(tree->serialize(builder));
    }

    auto trees_fb = builder.CreateVector(tree_vec);

    std::vector<flatbuffers::Offset<slamd::flatb::View>> view_vec;

    for (auto& [view_name, view] : this->view_name_to_view) {
        auto view_name_flatb = builder.CreateString(view_name);
        view_vec.push_back(slamd::flatb::CreateView(
            builder,
            view_name_flatb,
            view.tree->id.value,
            view.view_type
        ));
    }

    auto views_fb = builder.CreateVector(view_vec);

    auto state_fb = slamd::flatb::CreateInitialState(
        builder,
        vis_name_fb,
        views_fb,
        trees_fb
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
        asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 5555)
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
    std::string name
) {
    auto visualizer = std::make_shared<_vis::Visualizer>(name);

    // _global::visualizers.add(visualizer->id, visualizer);

    return visualizer;
}

}  // namespace slamd
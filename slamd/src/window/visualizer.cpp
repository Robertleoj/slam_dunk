#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <spdlog/spdlog.h>
#include <set>
#include <asio.hpp>
#include <slamd/render_thread_job_queue.hpp>
#include <slamd/visualizer.hpp>

namespace slamd {

Visualizer::Visualizer(
    std::string name
)
    : name(name) {
    this->server_thread = std::jthread([this](std::stop_token st) {
        this->server_job(st);
    });
}

void Visualizer::add_scene(
    std::string name,
    std::shared_ptr<Scene> scene
) {
    std::scoped_lock l(this->view_map_mutex);

    this->view_map.emplace(name, std::make_shared<SceneView>(scene));
}

void Visualizer::add_canvas(
    std::string name,
    std::shared_ptr<Canvas> canvas
) {
    std::scoped_lock l(this->view_map_mutex);

    this->view_map.emplace(name, std::make_shared<CanvasView>(canvas));
}



void Visualizer::server_job(
    std::stop_token& stop_token
) {
    asio::io_context io;
    asio::ip::tcp::acceptor acceptor(
        io,
        asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 5555)
    );

    std::set<std::shared_ptr<Connection>> clients;

    std::function<void(void)> accept_loop = [&]() {
        acceptor.async_accept([&](std::error_code ec,
                                  asio::ip::tcp::socket socket) {
            if (!ec) {
                auto conn = std::make_shared<Connection>(std::move(socket));
                clients.insert(conn);
            }
            accept_loop();  // keep accepting
        });
    };

    accept_loop();

    // // Simulate update loop in a thread
    // std::thread update_thread([&]() {
    //     while (true) {
    //         std::string update = "state_update_" + std::to_string(rand() % 100);
    //         for (auto& client : clients) {
    //             client->send_update(update);
    //         }
    //         std::this_thread::sleep_for(std::chrono::seconds(1));
    //     }
    // });

    io.run();
    // update_thread.join();
}

Visualizer::~Visualizer() {
    this->server_thread.request_stop();
}

}  // namespace slamd
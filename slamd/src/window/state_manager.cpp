#include <spdlog/spdlog.h>
#include <visualizer_generated.h>
#include <asio.hpp>
#include <slamd_window/state_manager.hpp>
#include <slamd_window/view/canvas_view.hpp>
#include <slamd_window/view/scene_view.hpp>
#include <slamd_window/view/view.hpp>

namespace slamdw {

StateManager::StateManager() {}

void StateManager::try_connect(
    std::string ip,
    ushort port
) {
    asio::io_context io;
    asio::ip::tcp::socket socket(io);

    socket.connect(asio::ip::tcp::endpoint(asio::ip::make_address(ip), port));

    uint32_t len_net;
    asio::read(socket, asio::buffer(&len_net, 4));
    uint32_t len = ntohl(len_net);
    spdlog::info("Reading {} bytes", len);

    std::vector<uint8_t> buf(len);

    asio::read(socket, asio::buffer(buf.data(), len));

    const slamd::flatb::VizFullState* full_state_fb =
        slamd::flatb::GetVizFullState(buf.data());

    this->name = full_state_fb->name()->str();

    std::shared_ptr<_tree::Tree> tree;

    auto trees_fb = full_state_fb->trees();

    spdlog::info("Found {} trees", trees_fb->size());

    for (size_t i = 0; i < trees_fb->size(); i++) {
        auto tree_fb = trees_fb->Get(i);
        uint64_t tree_id = tree_fb->id();

        switch (tree_fb->tree_type()) {
            case (slamd::flatb::TreeType_CANVAS): {
                tree = std::make_shared<Canvas>(tree_id);
                break;
            }
            case (slamd::flatb::TreeType_SCENE): {
                tree = std::make_shared<Scene>(tree_id);
                break;
            }
        }

        this->trees.insert({tree_id, tree});
    }

    auto views_fb = full_state_fb->views();
    spdlog::info("Found {} views", views_fb->size());

    for (size_t i = 0; i < views_fb->size(); i++) {
        auto view_fb = views_fb->Get(i);

        uint64_t tree_id = view_fb->tree_id();
        std::string view_name = view_fb->name()->str();

        auto tree = this->trees.at(tree_id);

        std::unique_ptr<View> view;

        if (auto canvas = std::dynamic_pointer_cast<Canvas>(tree)) {
            view = std::make_unique<CanvasView>(canvas);
        } else if (auto scene = std::dynamic_pointer_cast<Scene>(tree)) {
            view = std::make_unique<SceneView>(scene);
        } else {
            throw std::runtime_error("Invalid class");
        }

        this->views.insert({view_name, std::move(view)});
    }
    spdlog::info("loaded state");
}

}  // namespace slamdw
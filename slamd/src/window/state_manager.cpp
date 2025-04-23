#include <flatb/visualizer_generated.h>
#include <spdlog/spdlog.h>
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
    this->connection.emplace(ip, port);
}

void StateManager::handle_initial_state(
    const slamd::flatb::InitialState* full_state_fb
) {
    // const slamd::flatb::InitialState* full_state_fb =
    // slamd::flatb::GetInitialState(data.data());

    this->name = full_state_fb->name()->str();

    std::shared_ptr<Tree> tree;

    auto trees_fb = full_state_fb->trees();

    spdlog::info("Found {} trees", trees_fb->size());

    for (size_t i = 0; i < trees_fb->size(); i++) {
        auto tree_fb = trees_fb->Get(i);
        uint64_t tree_id = tree_fb->id();

        tree = std::make_shared<Tree>(tree_id);
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

        switch (view_fb->view_type()) {
            case (slamd::flatb::ViewType_CANVAS): {
                view = std::make_unique<CanvasView>(tree);
                break;
            }
            case (slamd::flatb::ViewType_SCENE): {
                view = std::make_unique<SceneView>(tree);
                break;
            }
        }

        this->views.insert({view_name, std::move(view)});
    }
    spdlog::info("loaded state");
}

void StateManager::apply_updates() {
    if (!this->connection.has_value()) {
        return;
    }

    auto& connection = this->connection.value();
    auto& message_queue = connection.messages;

    while (true) {
        auto maybe_message = message_queue.try_pop();
        if (!maybe_message.has_value()) {
            return;
        }
        auto& message = maybe_message.value();

        auto message_fb = message.msg();

        switch (message_fb->message_type()) {
            case (slamd::flatb::MessageUnion_initial_state): {
                this->handle_initial_state(message_fb->message_as_initial_state(
                ));
                break;
            }
            default: {
                spdlog::info(
                    "Unknown message type {}",
                    static_cast<uint32_t>(message_fb->message_type())
                );
            }
        }
    }
}

}  // namespace slamdw
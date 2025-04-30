#include <flatb/visualizer_generated.h>
#include <spdlog/spdlog.h>
#include <asio.hpp>
#include <slamd_window/state_manager.hpp>
#include <slamd_window/view/canvas_view.hpp>
#include <slamd_window/view/scene_view.hpp>
#include <slamd_window/view/view.hpp>

namespace slamd {

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

    auto trees_fb = full_state_fb->trees();

    spdlog::debug("Found {} trees", trees_fb->size());

    for (auto geom_fb : *full_state_fb->geometries()) {
        auto geom = _geom::Geometry::deserialize(geom_fb);
        this->geometries.insert({_id::GeometryID(geom_fb->geometry_id()), geom}
        );
    }

    for (auto tree_fb : *trees_fb) {
        auto tree_id = _id::TreeID(tree_fb->id());

        this->trees.insert(
            {tree_id, Tree::deserialize(tree_fb, this->geometries)}
        );
    }

    auto views_fb = full_state_fb->views();
    spdlog::debug("Found {} views", views_fb->size());

    for (auto view_fb : *views_fb) {
        auto tree_id = _id::TreeID(view_fb->tree_id());
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
            default: {
                throw std::runtime_error("Invalid geometry type");
            }
        }

        this->views.insert({view_name, std::move(view)});
    }
    spdlog::debug("loaded state");
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

}  // namespace slamd
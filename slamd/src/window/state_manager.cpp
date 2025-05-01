#include <flatb/visualizer_generated.h>
#include <spdlog/spdlog.h>
#include <asio.hpp>
#include <slamd_common/gmath/serialization.hpp>
#include <slamd_common/gmath/stringify.hpp>
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

        auto view = View::deserialize(view_fb, tree);

        this->views.insert({view_name, std::move(view)});
    }
    spdlog::debug("loaded state");
}

void StateManager::handle_set_transform(
    const slamd::flatb::SetTransform* set_transform_fb
) {
    auto tree_id = _id::TreeID(set_transform_fb->tree_id());

    auto tree = this->trees.at(tree_id);

    TreePath path(set_transform_fb->tree_path()->str());

    auto transform = gmath::deserialize(set_transform_fb->transform());

    tree->set_transform(path, transform);
}

void StateManager::handle_set_object(
    const slamd::flatb::SetObject* set_object_fb
) {
    auto obj =
        this->geometries.at(_id::GeometryID(set_object_fb->geometry_id()));

    TreePath path(set_object_fb->tree_path()->str());
    auto tree_id = set_object_fb->tree_id();

    auto tree = this->trees.at(_id::TreeID(tree_id));

    tree->set_object(path, obj);
}

void StateManager::handle_add_geometry(
    const slamd::flatb::AddGeometry* add_geometry_fb
) {
    auto geometry = _geom::Geometry::deserialize(add_geometry_fb->geometry());
    auto geometry_id =
        _id::GeometryID(add_geometry_fb->geometry()->geometry_id());

    this->geometries.insert({geometry_id, geometry});
}

void StateManager::handle_remove_geometry(
    const slamd::flatb::RemoveGeometry* remove_geometry_fb
) {
    auto geometry_id = _id::GeometryID(remove_geometry_fb->geometry_id());
    this->geometries.erase(geometry_id);
}

void StateManager::handle_add_tree(
    const slamd::flatb::AddTree* add_tree_fb
) {
    auto tree = Tree::deserialize(add_tree_fb->tree(), this->geometries);

    this->trees.insert({_id::TreeID(add_tree_fb->tree()->id()), tree});
}
void StateManager::handle_add_view(
    const slamd::flatb::AddView* add_view_fb
) {
    auto view_fb = add_view_fb->view();
    auto tree_id = _id::TreeID(view_fb->tree_id());

    auto tree = this->trees.at(tree_id);

    auto view = View::deserialize(view_fb, tree);
    auto view_name = view_fb->name()->str();

    this->views.insert({view_name, std::move(view)});
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
        auto message = std::move(maybe_message.value());

        auto message_fb = message->msg();

        switch (message_fb->message_type()) {
            case (slamd::flatb::MessageUnion_initial_state): {
                this->handle_initial_state(message_fb->message_as_initial_state(
                ));
                break;
            }
            case (slamd::flatb::MessageUnion_set_transform): {
                this->handle_set_transform(message_fb->message_as_set_transform(
                ));
                break;
            }
            case (slamd::flatb::MessageUnion_set_object): {
                this->handle_set_object(message_fb->message_as_set_object());
                break;
            }
            case (slamd::flatb::MessageUnion_add_geometry): {
                this->handle_add_geometry(message_fb->message_as_add_geometry()
                );
                break;
            }
            case (slamd::flatb::MessageUnion_remove_geometry): {
                this->handle_remove_geometry(
                    message_fb->message_as_remove_geometry()
                );
                break;
            }
            case (slamd::flatb::MessageUnion_add_tree): {
                this->handle_add_tree(message_fb->message_as_add_tree());
                break;
            }
            case (slamd::flatb::MessageUnion_add_view): {
                this->handle_add_view(message_fb->message_as_add_view());
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
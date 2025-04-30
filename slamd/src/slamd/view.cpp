#include <slamd/view.hpp>

namespace slamd {
namespace _view {

std::shared_ptr<View> View::create(
    std::shared_ptr<_vis::Visualizer> vis,
    std::shared_ptr<_tree::Tree> tree,
    slamd::flatb::ViewType view_type
) {
    auto view = std::shared_ptr<View>(new View(vis, tree, view_type));
    tree->attached_to.insert({view->id, view->shared_from_this()});

    return view;
}

void View::broadcast(
    std::shared_ptr<std::vector<uint8_t>> message_buffer
) {
    auto vis = this->vis.lock();
    if (!vis) {
        throw std::runtime_error("Visualizer not valid! Should never happen.");
    }

    vis->broadcast(message_buffer);
}

std::map<_id::VisualizerID, std::shared_ptr<_vis::Visualizer>>
View::find_visualizers() {
    auto vis = this->vis.lock();
    if (!vis) {
        throw std::runtime_error("Visualizer not valid! Should never happen.");
    }

    return {{vis->id, vis}};
}

View::View(
    std::shared_ptr<_vis::Visualizer> vis,
    std::shared_ptr<_tree::Tree> tree,
    slamd::flatb::ViewType view_type
)
    : tree(tree),
      view_type(view_type),
      vis(vis) {}

View::~View() {
    this->tree->attached_to.erase(this->id);
}

}  // namespace _view
}  // namespace slamd
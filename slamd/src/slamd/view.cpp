#include <slamd/view.hpp>

namespace slamd {
namespace _view {

std::shared_ptr<View> View::create(
    _vis::Visualizer* vis,
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
    this->vis->broadcast(message_buffer);
}

View::View(
    _vis::Visualizer* vis,
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
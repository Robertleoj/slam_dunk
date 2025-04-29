#include <slamd/view.hpp>

#include <slamd/object_tracker.hpp>
namespace slamd {
namespace _view {

std::shared_ptr<View> View::create(
    _vis::Visualizer* vis,
    std::shared_ptr<_tree::Tree> tree,
    slamd::flatb::ViewType view_type
) {
    auto view = std::shared_ptr<View>(new View(vis, tree, view_type));
    _global::views.add(view);

    return view;
}

View::View(
    _vis::Visualizer* vis,
    std::shared_ptr<_tree::Tree> tree,
    slamd::flatb::ViewType view_type
)
    : tree(tree),
      view_type(view_type),
      vis(vis) {
    tree->attached_to.insert(this->id);
}

View::~View() {
    this->tree->attached_to.erase(this->id);

    _global::views.remove(this->id);
}

}  // namespace _view
}  // namespace slamd
#include <slamd/view.hpp>

namespace slamd {
namespace _view {

// View::View()
//     : id(id_counter++) {}

View::View(
    _vis::Visualizer* vis,
    std::shared_ptr<_tree::Tree> tree,
    slamd::flatb::ViewType view_type
)
    : id(_id::ViewID::next()),
      tree(tree),
      view_type(view_type),
      vis(vis) {
    tree->attached_to.insert(this->id);
}

View::~View() {
    this->tree->attached_to.erase(this->id);
}

}  // namespace _view
}  // namespace slamd
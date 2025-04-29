#include <slamd/view.hpp>

namespace slamd {
namespace _view {

// View::View()
//     : id(id_counter++) {}

View::View(
    _id::TreeID tree_id,
    slamd::flatb::ViewType view_type
)
    : id(_id::ViewID::next()),
      tree_id(tree_id),
      view_type(view_type) {}

}  // namespace _view
}  // namespace slamd
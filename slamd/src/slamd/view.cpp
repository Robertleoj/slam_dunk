#include <slamd/view.hpp>

namespace slamd {

std::atomic<uint64_t> View::id_counter;

// View::View()
//     : id(id_counter++) {}

View::View(
    uint64_t tree_id,
    slamd::flatb::ViewType view_type
)
    : id(View::id_counter++),
      tree_id(tree_id),
      view_type(view_type) {}

}  // namespace slamd
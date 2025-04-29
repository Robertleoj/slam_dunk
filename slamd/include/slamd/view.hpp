#pragma once
#include <flatb/visualizer_generated.h>
#include <memory>
#include <slamd/id.hpp>
#include <slamd/tree/tree.hpp>

namespace slamd {

namespace _id {
struct ViewTag {};
using ViewID = ID<ViewTag>;
}  // namespace _id
namespace _view {

class View : std::enable_shared_from_this<View> {
   public:
    View(_id::TreeID tree_id, slamd::flatb::ViewType view_type);
    const _id::TreeID tree_id;
    slamd::flatb::ViewType view_type;

    const _id::ViewID id;

   private:
};

}  // namespace _view
}  // namespace slamd
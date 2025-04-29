#pragma once
#include <flatb/visualizer_generated.h>
#include <memory>
#include <slamd/id.hpp>
#include <slamd/tree/tree.hpp>

namespace slamd {

namespace _view {
class View : std::enable_shared_from_this<View> {
   public:
    View(std::shared_ptr<_tree::Tree> tree, slamd::flatb::ViewType view_type);
    ~View();

   public:
    const std::shared_ptr<_tree::Tree> tree;
    slamd::flatb::ViewType view_type;

    const _id::ViewID id;
};

}  // namespace _view
}  // namespace slamd
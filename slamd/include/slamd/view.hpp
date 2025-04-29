#pragma once
#include <flatb/visualizer_generated.h>
#include <memory>
#include <slamd/id.hpp>
#include <slamd/tree/tree.hpp>
#include <slamd/visualizer.hpp>

namespace slamd {

namespace _vis {
class Visualizer;
}

namespace _view {
class View {
   public:
    View(
        _vis::Visualizer*,
        std::shared_ptr<_tree::Tree> tree,
        slamd::flatb::ViewType view_type
    );
    ~View();

   public:
    const std::shared_ptr<_tree::Tree> tree;
    slamd::flatb::ViewType view_type;
    _vis::Visualizer* vis;

    const _id::ViewID id;
};

}  // namespace _view
}  // namespace slamd
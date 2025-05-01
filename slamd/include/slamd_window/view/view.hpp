#pragma once
#include <flatb/visualizer_generated.h>
#include <slamd_window/tree/tree.hpp>

namespace slamd {

class View {
   public:
    virtual void render_to_imgui() = 0;  // pure virtual = abstract base class
    virtual ~View() = default;  // virtual dtor for safe polymorphic deletion
    static std::unique_ptr<View>
    deserialize(const flatb::View* view, std::shared_ptr<Tree> tree);
};

}  // namespace slamd
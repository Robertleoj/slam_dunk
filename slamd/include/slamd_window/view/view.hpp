#pragma once
#include <flatb/visualizer_generated.h>
#include <slamd_window/tree/tree.hpp>

namespace slamd {

class View {
   public:
    View(std::shared_ptr<Tree> t);
    virtual void render_to_imgui() = 0;  // pure virtual = abstract base class
    virtual ~View() = default;  // virtual dtor for safe polymorphic deletion
    static std::unique_ptr<View>
    deserialize(const flatb::View* view, std::shared_ptr<Tree> tree);
    std::shared_ptr<Tree> tree;

    std::optional<std::string> visualize_glob = std::nullopt;
    char filter_buf[512] = "";
};

}  // namespace slamd
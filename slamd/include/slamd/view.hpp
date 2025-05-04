#pragma once
#include <flatb/visualizer_generated.h>
#include <memory>
#include <slamd/tree/tree.hpp>
#include <slamd/visualizer.hpp>
#include <slamd_common/id.hpp>

namespace slamd {

namespace _vis {
class Visualizer;
}

namespace _tree {
class Tree;
}

namespace _view {
class View : public std::enable_shared_from_this<View> {
   public:
    static std::shared_ptr<View> create(
        std::string name,
        std::shared_ptr<_vis::Visualizer> vis,
        std::shared_ptr<_tree::Tree> tree,
        slamd::flatb::ViewType view_type
    );
    ~View();

    void broadcast(std::shared_ptr<std::vector<uint8_t>> message_buffer);

    std::map<_id::VisualizerID, std::shared_ptr<_vis::Visualizer>>
    find_visualizers();

    flatbuffers::Offset<flatb::View> serialize(
        flatbuffers::FlatBufferBuilder& builder
    );
    std::shared_ptr<std::vector<uint8_t>> get_add_view_message();
    std::shared_ptr<std::vector<uint8_t>> get_remove_view_message();

   private:
    View(
        std::string name,
        std::shared_ptr<_vis::Visualizer> vis,
        std::shared_ptr<_tree::Tree> tree,
        slamd::flatb::ViewType view_type
    );

   public:
    const std::shared_ptr<_tree::Tree> tree;
    slamd::flatb::ViewType view_type;

    // a view is contained in the visualizer, so the lifetime is bound to it
    std::weak_ptr<_vis::Visualizer> vis;

    std::string name;
    const _id::ViewID id;
};

}  // namespace _view
}  // namespace slamd
#pragma once
#include <flatb/visualizer_generated.h>
#include <glm/glm.hpp>
#include <map>
#include <memory>
#include <slamd/geom/geometry.hpp>
#include <slamd/tree/tree_path.hpp>
#include <slamd/view.hpp>
#include <slamd_common/id.hpp>

namespace slamd {

namespace _vis {
class Visualizer;
}
namespace _view {
class View;
}

namespace _geom {
class Geometry;
}

namespace _tree {

class Tree;

class Node : public std::enable_shared_from_this<Node> {
   public:
    ~Node();
    Node(Tree* tree, TreePath path);

    std::optional<glm::mat4> get_transform() const;

    void set_object(std::shared_ptr<_geom::Geometry> object);

    void set_transform(glm::mat4 transform);

    Node() = delete;
    Node(const Node&) = delete;
    Node(Node&&) = delete;
    Node& operator=(const Node&) = delete;

    // should not be in public API
    std::optional<std::shared_ptr<_geom::Geometry>> get_object() const;

    flatbuffers::Offset<slamd::flatb::Node> serialize(
        flatbuffers::FlatBufferBuilder& builder
    );

    void broadcast(std::shared_ptr<std::vector<uint8_t>> message_buffer);

    std::map<_id::VisualizerID, std::shared_ptr<_vis::Visualizer>>
    find_visualizers();

   public:
    std::map<std::string, std::shared_ptr<Node>> children;
    const _id::NodeID id;

   private:
    std::optional<glm::mat4> transform;
    std::optional<std::shared_ptr<_geom::Geometry>> object;

    mutable std::mutex transform_mutex;
    mutable std::mutex object_mutex;

    // we use a raw tree pointer here as the lifetime of the node
    // is tied to the tree.
    Tree* tree;
    const TreePath path;
};

class Tree {
   public:
    Tree();

    virtual void set_object(
        const std::string& path,
        std::shared_ptr<_geom::Geometry> object
    );

    virtual flatbuffers::Offset<slamd::flatb::Tree> serialize(
        flatbuffers::FlatBufferBuilder& builder
    );

    void add_all_geometries(
        std::map<_id::GeometryID, std::shared_ptr<_geom::Geometry>>& initial_map
    );

    void broadcast(std::shared_ptr<std::vector<uint8_t>> message_buffer);
    std::map<_id::VisualizerID, std::shared_ptr<_vis::Visualizer>>
    find_visualizers();

   protected:
    void
    set_transform_mat4(const std::string& path, const glm::mat4& transform);

    std::optional<Node*> traverse(const TreePath& path);
    Node* make_path(TreePath path);

   public:
    const _id::TreeID id;
    std::map<_id::ViewID, std::weak_ptr<_view::View>> attached_to;

   private:
    void add_all_geometries_rec(
        Node* node,
        std::map<_id::GeometryID, std::shared_ptr<_geom::Geometry>>& initial_map
    );

   private:
    std::shared_ptr<Node> root;
};

}  // namespace _tree

/**
 * 3D version
 */
class Scene : public _tree::Tree {
   public:
    void set_transform(const std::string& path, glm::mat4 transform);
};

std::shared_ptr<Scene> scene();

/**
 * 2D version
 */
class Canvas : public _tree::Tree {
   public:
    void set_transform(const std::string& path, glm::mat3 transform);
    void set_object(
        const std::string& path,
        std::shared_ptr<_geom::Geometry> object
    ) override;

   private:
    uint64_t insertion_order_counter;
    float new_depth();
};

std::shared_ptr<Canvas> canvas();

}  // namespace slamd
#pragma once
#include <flatb/visualizer_generated.h>
#include <glm/glm.hpp>
#include <map>
#include <memory>
#include <slamd/geom/geometry.hpp>
#include <slamd/id.hpp>
#include <slamd/tree/tree_path.hpp>

namespace slamd {

namespace _tree {

class Tree;

class Node {
   public:
    Node(Tree* tree);
    std::optional<std::shared_ptr<_geom::Geometry>> get_object() const;

    flatbuffers::Offset<slamd::flatb::Node> serialize(
        flatbuffers::FlatBufferBuilder& builder
    );

    std::optional<glm::mat4> get_transform() const;

    void set_object(std::shared_ptr<_geom::Geometry> object);

    void set_transform(glm::mat4 transform);

    ~Node();

   public:
    std::map<std::string, std::unique_ptr<Node>> children;
    const _id::NodeID id;

   private:
    std::optional<glm::mat4> transform;
    std::optional<std::shared_ptr<_geom::Geometry>> object;

    mutable std::mutex transform_mutex;
    mutable std::mutex object_mutex;

    // we use a raw tree pointer here as the lifetime of the node
    // is tied to the tree.
    const Tree* tree;
};

class Tree {
   private:
    std::unique_ptr<Node> root;

   public:
    const _id::TreeID id;
    Tree();

    virtual void set_object(
        const std::string& path,
        std::shared_ptr<_geom::Geometry> object
    );

    virtual flatbuffers::Offset<slamd::flatb::Tree> serialize(
        flatbuffers::FlatBufferBuilder& builder
    );

    std::set<_id::ViewID> attached_to;

   protected:
    void
    set_transform_mat4(const std::string& path, const glm::mat4& transform);

    std::optional<Node*> traverse(const TreePath& path);
    Node* make_path(TreePath path);
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
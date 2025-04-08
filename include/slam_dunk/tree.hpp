#pragma once
#include <glm/glm.hpp>
#include <slam_dunk/node.hpp>
#include <slam_dunk/scene_object.hpp>
#include <slam_dunk/tree_path.hpp>

namespace slamd {

class Tree {
   private:
    std::unique_ptr<Node> tree_root;

   public:
    Tree();
    void set_object(const TreePath& path, std::shared_ptr<SceneObject> object);
    void
    set_object_weak(const TreePath& path, std::weak_ptr<SceneObject> object);
    void set_transform(const TreePath& path, const glm::mat4& transform);

    void render(const glm::mat4 view, const glm::mat4 projection) const;

   private:
    void render_recursive(
        const Node* node,
        const glm::mat4 current_transform,
        const glm::mat4& view,
        const glm::mat4& projection
    ) const;

    Node* make_path(TreePath path);

    // std::optional<Node *> traverse_to(TreePath path, bool create_path);
};
}  // namespace slamd
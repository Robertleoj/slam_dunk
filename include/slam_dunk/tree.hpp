#pragma once
#include <glm/glm.hpp>
#include <slam_dunk/node.hpp>
#include <slam_dunk/scene_object.hpp>
#include <slam_dunk/tree_path.hpp>

namespace sdunk {

class Tree {

   public:
    Tree();
    void set_object(TreePath path, std::shared_ptr<SceneObject> object);
    void set_object(TreePath path, std::weak_ptr<SceneObject> object);
    void set_transform(TreePath path, glm::mat4 transform);

   private:
    std::unique_ptr<Node> tree_root;
    Node * make_path(TreePath path);
    // std::optional<Node *> traverse_to(TreePath path, bool create_path);
};
}  // namespace sdunk
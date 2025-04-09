#pragma once
#include <glm/glm.hpp>
#include <slamd/geometry/geometry.hpp>
#include <slamd/tree/node.hpp>
#include <slamd/tree_path.hpp>

namespace slamd {
namespace _tree {

class Tree {
   private:
    std::unique_ptr<Node> root;

   public:
    Tree();

    void set_object(
        const TreePath& path,
        std::shared_ptr<_geometry::Geometry> object
    );

    void render(const glm::mat4& view, const glm::mat4& projection) const;

    void set_object_weak(
        const TreePath& path,
        std::weak_ptr<_geometry::Geometry> object
    );

   protected:
    void set_transform_mat4(const TreePath& path, const glm::mat4& transform);

   private:
    void render_recursive(
        const Node* node,
        const glm::mat4 current_transform,
        const glm::mat4& view,
        const glm::mat4& projection
    ) const;

    Node* make_path(TreePath path);
};

}  // namespace _tree

/**
 * 3D version
 */
class Scene : public _tree::Tree {
   public:
    void set_transform(const TreePath& path, glm::mat4 transform);
};

std::shared_ptr<Scene> scene();

/**
 * 2D version
 */
class Canvas : public _tree::Tree {
   public:
    void set_transform(const TreePath& path, glm::mat3 transform);
};

std::shared_ptr<Canvas> canvas();

}  // namespace slamd
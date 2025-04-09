#pragma once
#include <glm/glm.hpp>
#include <slamd/geometry/geometry.hpp>
#include <slamd/scene_node.hpp>
#include <slamd/tree_path.hpp>

namespace slamd {

class Scene {
   private:
    std::unique_ptr<SceneNode> tree_root;

   public:
    Scene();
    void set_object(
        const TreePath& path,
        std::shared_ptr<geometry::Geometry> object
    );
    void set_object_weak(
        const TreePath& path,
        std::weak_ptr<geometry::Geometry> object
    );
    void set_transform(const TreePath& path, const glm::mat4& transform);

    void render(const glm::mat4 view, const glm::mat4 projection) const;

   private:
    void render_recursive(
        const SceneNode* node,
        const glm::mat4 current_transform,
        const glm::mat4& view,
        const glm::mat4& projection
    ) const;

    SceneNode* make_path(TreePath path);
};

std::shared_ptr<Scene> scene();

}  // namespace slamd
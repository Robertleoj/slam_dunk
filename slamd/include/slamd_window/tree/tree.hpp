#pragma once
#include <glm/glm.hpp>
#include <slamd_common/gmath/aabb.hpp>
#include <slamd_window/geom/geometry.hpp>
#include <slamd_window/tree/node.hpp>
#include <slamd_window/tree/tree_path.hpp>

namespace slamdw {

class Tree {
   private:
    uint64_t id;
    std::unique_ptr<Node> root;

   public:
    Tree(uint64_t id);

    virtual void set_object(
        const std::string& path,
        std::shared_ptr<_geom::Geometry> object
    );

    void render(const glm::mat4& view, const glm::mat4& projection) const;

    std::optional<slamd::gmath::AABB> bounds();

   protected:
    void
    set_transform_mat4(const std::string& path, const glm::mat4& transform);

    std::optional<Node*> traverse(const TreePath& path);
    Node* make_path(TreePath path);

   private:
    void render_recursive(
        const Node* node,
        const glm::mat4& current_transform,
        const glm::mat4& view,
        const glm::mat4& projection
    ) const;

    std::optional<slamd::gmath::AABB>

    bounds_recursive(const Node* node, const glm::mat4& prev_transform);
};

}  // namespace slamdw
#pragma once
#include <glm/glm.hpp>
#include <slamd/geometry/geometry.hpp>
#include <slamd/geometry2d/geometry2d.hpp>
#include <slamd/tree/node.hpp>
#include <slamd/tree_path.hpp>

namespace slamd {
namespace _tree {

template <typename D>
class Tree {
   private:
    std::unique_ptr<Node<D>> root;

   public:
    Tree() { this->root = std::make_unique<Node<D>>(); }

    void set_object(
        const TreePath& path,
        std::shared_ptr<D> object
    ) {
        if (path.is_root()) {
            throw std::runtime_error("Setting root object is not allowed");
        }

        Node<D>* node = this->make_path(path);
        node->set_object(object);
    }

    void render(
        const glm::mat4& view,
        const glm::mat4& projection
    ) const {
        this->render_recursive(
            this->root.get(), glm::mat4(1.0), view, projection
        );
    }

    void set_object_weak(
        const TreePath& path,
        std::weak_ptr<D> object
    ) {
        if (path.is_root()) {
            throw std::runtime_error("Setting root object is not allowed");
        }

        Node<D>* node = this->make_path(path);
        node->set_object(object);
    }

   protected:
    void set_transform_mat4(
        const TreePath& path,
        const glm::mat4& transform
    ) {
        if (path.is_root()) {
            throw std::runtime_error("Setting root transform is not allowed");
        }

        Node<D>* node = this->make_path(path);
        node->set_transform(transform);
    }

   private:
    void render_recursive(
        const Node<D>* node,
        const glm::mat4 current_transform,
        const glm::mat4& view,
        const glm::mat4& projection
    ) const {
        glm::mat4 next_transform = current_transform;
        auto node_transform = node->get_transform();

        if (node_transform.has_value()) {
            next_transform = current_transform * node_transform.value();
        }

        const auto node_object = node->get_object();

        if (node_object.has_value()) {
            node_object.value()->render(next_transform, view, projection);
        }

        for (auto& [_, child] : node->children) {
            this->render_recursive(
                child.get(), next_transform, view, projection
            );
        }
    }

    Node<D>* make_path(
        TreePath path
    ) {
        if (path.components.size() == 0) {
            return this->root.get();
        }

        Node<D>* current_node = root.get();

        for (size_t i = 0; i < path.components.size(); i++) {
            auto& component = path.components[i];

            auto child_iterator = current_node->children.find(component);

            if (child_iterator == current_node->children.end()) {
                // in this case, we want to create the path down to the target
                // node
                std::unique_ptr<Node<D>> new_node = std::make_unique<Node<D>>();
                Node<D>* new_node_ptr = new_node.get();

                // insert the new child
                current_node->children.emplace(component, std::move(new_node));

                current_node = new_node_ptr;

            } else {
                // node exists, just use it as the current node
                current_node = child_iterator->second.get();
            }
        }

        return current_node;
    }
};

}  // namespace _tree

/**
 * 3D version
 */
class Scene : public _tree::Tree<geometry::Geometry> {
   public:
    void set_transform(const TreePath& path, glm::mat4 transform);
};

std::shared_ptr<Scene> scene();

/**
 * 2D version
 */
class Canvas : public _tree::Tree<geometry2d::Geometry2D> {
   public:
    void set_transform(const TreePath& path, glm::mat3 transform);
};

std::shared_ptr<Canvas> canvas();

}  // namespace slamd
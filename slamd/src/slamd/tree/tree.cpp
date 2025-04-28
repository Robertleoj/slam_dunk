#include <spdlog/spdlog.h>
#include <slamd/global_object_map.hpp>
#include <slamd/tree/tree.hpp>
#include <slamd_common/gmath/transforms.hpp>

namespace slamd {

namespace _tree {

std::atomic<uint64_t> Tree::id_counter = 1;

Tree::Tree()
    : id(Tree::id_counter++) {
    this->root = std::make_unique<Node>();
}

void Tree::set_object(
    const std::string& path,
    std::shared_ptr<_geom::Geometry> object
) {
    TreePath treepath(path);

    if (treepath.is_root()) {
        throw std::runtime_error("Setting root object is not allowed");
    }

    Node* node = this->make_path(treepath);
    node->set_object(object);
}

flatbuffers::Offset<slamd::flatb::Tree> Tree::serialize(
    flatbuffers::FlatBufferBuilder& builder
) {
    auto serialized_root = this->root->serialize(builder);
    return slamd::flatb::CreateTree(builder, this->id, serialized_root);
}

void Tree::set_transform_mat4(
    const std::string& path,
    const glm::mat4& transform
) {
    TreePath treepath(path);
    if (treepath.is_root()) {
        throw std::runtime_error("Setting root transform is not allowed");
    }

    Node* node = this->make_path(treepath);
    node->set_transform(transform);
}

std::optional<Node*> Tree::traverse(
    const TreePath& path
) {
    Node* current_node = root.get();

    for (size_t i = 0; i < path.components.size(); i++) {
        auto& component = path.components[i];

        auto child_iterator = current_node->children.find(component);

        if (child_iterator == current_node->children.end()) {
            return std::nullopt;
        }

        current_node = child_iterator->second.get();
    }

    return current_node;
}

Node* Tree::make_path(
    TreePath path
) {
    if (path.components.size() == 0) {
        return this->root.get();
    }

    Node* current_node = root.get();

    for (size_t i = 0; i < path.components.size(); i++) {
        auto& component = path.components[i];

        auto child_iterator = current_node->children.find(component);

        if (child_iterator == current_node->children.end()) {
            // in this case, we want to create the path down to the target
            // node
            std::unique_ptr<Node> new_node = std::make_unique<Node>();
            Node* new_node_ptr = new_node.get();

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

}  // namespace _tree

void Scene::set_transform(
    const std::string& path,
    glm::mat4 transform
) {
    this->set_transform_mat4(path, transform);
}

std::shared_ptr<Scene> scene() {
    auto scene = std::make_shared<Scene>();
    _global::trees.add(scene->id, scene);
    return scene;
}

void Canvas::set_transform(
    const std::string& path,
    glm::mat3 transform
) {
    auto new_transform = gmath::xy_to_3d(transform);
    auto node = this->make_path(path);

    auto node_transform = node->get_transform().value_or(glm::mat4(1.0));

    new_transform[3][2] = node_transform[3][2];

    node->set_transform(new_transform);
}

void Canvas::set_object(
    const std::string& path,
    std::shared_ptr<_geom::Geometry> object
) {
    auto node = this->make_path(path);

    bool is_first_insert = !node->get_object().has_value();

    node->set_object(object);

    if (is_first_insert) {
        auto node_transform = node->get_transform().value_or(glm::mat4(1.0));

        node_transform[3][2] = this->new_depth();

        node->set_transform(node_transform);
    }
}

float Canvas::new_depth() {
    float new_depth =
        static_cast<float>(this->insertion_order_counter) / 100.0f;
    this->insertion_order_counter += 1;
    return new_depth;
}

std::shared_ptr<Canvas> canvas() {
    auto canvas = std::make_shared<Canvas>();
    _global::trees.add(canvas->id, canvas);
    return canvas;
}

}  // namespace slamd
#include <spdlog/spdlog.h>

#include <flatb/messages_generated.h>
#include <memory>
#include <slamd/tree/tree.hpp>
#include <slamd_common/gmath/serialization.hpp>
#include <slamd_common/gmath/transforms.hpp>
#include <slamd_common/utils/serialization.hpp>
#include <vector>

namespace slamd {

namespace _tree {

Node::Node(
    Tree* tree,
    TreePath path
)
    : tree(tree),
      path(path) {}

Node::~Node() {
    if (this->object.has_value()) {
        this->object.value()->detach(this);
    }
}

std::optional<std::shared_ptr<_geom::Geometry>> Node::get_object() const {
    std::scoped_lock l(this->object_mutex);
    if (!this->object.has_value()) {
        return std::nullopt;
    }

    return this->object.value();
}

std::optional<glm::mat4> Node::get_transform() const {
    std::scoped_lock l(this->transform_mutex);
    return this->transform;
}

void Node::set_object(
    std::shared_ptr<_geom::Geometry> object
) {
    std::scoped_lock l(this->object_mutex);

    if (this->object.has_value()) {
        this->object.value()->detach(this);
    }
    this->object.emplace(object);

    object->attach(this->shared_from_this());

    // broadcast the attach
    flatbuffers::FlatBufferBuilder builder;

    auto path_fb = builder.CreateString(this->path.string());
    auto set_object_fb = flatb::CreateSetObject(
        builder,
        this->tree->id.value,
        path_fb,
        object->id.value
    );
    auto message_fb = flatb::CreateMessage(
        builder,
        flatb::MessageUnion_set_object,
        set_object_fb.Union()
    );
    builder.Finish(message_fb);
    this->broadcast(_utils::builder_buffer(builder));
}

void Tree::clear(
    const std::string& path
) {
    TreePath tree_path(path);

    auto current_node = this->root.get();

    for (size_t i = 0; i < tree_path.components.size(); i++) {
        auto& component = tree_path.components[i];

        if (i == tree_path.components.size() - 1) {
            // we are at the parent - we delete now
            current_node->children.erase(component);
            break;
        }
        auto it = current_node->children.find(component);
        if (it == current_node->children.end()) {
            // nothing to do
            return;
        }

        current_node = it->second.get();
    }

    if (tree_path.is_root()) {
        // special case - we want to unhook all children from the root
        this->root->children.clear();
    }

    this->broadcast(this->get_clear_path_message(path));
}

std::shared_ptr<std::vector<uint8_t>> Tree::get_clear_path_message(
    const std::string& path
) {
    flatbuffers::FlatBufferBuilder builder;

    auto path_fb = builder.CreateString(path);

    auto clear_path_fb =
        flatb::CreateClearPath(builder, this->id.value, path_fb);

    auto message_fb = flatb::CreateMessage(
        builder,
        flatb::MessageUnion_clear_path,
        clear_path_fb.Union()
    );

    builder.Finish(message_fb);

    return _utils::builder_buffer(builder);
}

std::shared_ptr<std::vector<uint8_t>> Tree::get_add_tree_message() {
    flatbuffers::FlatBufferBuilder builder;

    auto tree_fb = this->serialize(builder);

    auto add_tree_fb = flatb::CreateAddTree(builder, tree_fb);

    auto message_fb = flatb::CreateMessage(
        builder,
        flatb::MessageUnion_add_tree,
        add_tree_fb.Union()
    );

    builder.Finish(message_fb);

    return _utils::builder_buffer(builder);
}

std::shared_ptr<std::vector<uint8_t>> Tree::get_remove_tree_message() {
    flatbuffers::FlatBufferBuilder builder;

    auto remove_tree_fb = flatb::CreateRemoveTree(builder, this->id.value);

    auto message_fb = flatb::CreateMessage(
        builder,
        flatb::MessageUnion_remove_tree,
        remove_tree_fb.Union()
    );

    builder.Finish(message_fb);

    return _utils::builder_buffer(builder);
}

void Tree::add_all_geometries_rec(
    Node* node,
    std::map<_id::GeometryID, std::shared_ptr<_geom::Geometry>>& initial_map
) {
    for (auto& [_, child] : node->children) {
        this->add_all_geometries_rec(child.get(), initial_map);
    }

    auto geom_opt = node->get_object();

    if (!geom_opt.has_value()) {
        return;
    }

    auto geom = geom_opt.value();

    auto it = initial_map.find(geom->id);

    if (it != initial_map.end()) {
        return;
    }

    initial_map.insert({geom->id, geom});
}

std::map<_id::VisualizerID, std::shared_ptr<_vis::Visualizer>>
Node::find_visualizers() {
    return this->tree->find_visualizers();
}

std::map<_id::VisualizerID, std::shared_ptr<_vis::Visualizer>>
Tree::find_visualizers() {
    std::map<_id::VisualizerID, std::shared_ptr<_vis::Visualizer>> map;

    for (auto it = this->attached_to.begin(); it != this->attached_to.end();) {
        if (auto view = it->second.lock()) {
            auto view_vis_map = view->find_visualizers();
            map.insert(view_vis_map.begin(), view_vis_map.end());
            it++;
        } else {
            it = attached_to.erase(it);
        }
    }

    return map;
}

void Tree::add_all_geometries(
    std::map<_id::GeometryID, std::shared_ptr<_geom::Geometry>>& initial_map
) {
    this->add_all_geometries_rec(this->root.get(), initial_map);
}

flatbuffers::Offset<slamd::flatb::Node> Node::serialize(
    flatbuffers::FlatBufferBuilder& builder
) {
    std::vector<flatbuffers::Offset<slamd::flatb::ChildEntry>> children;

    for (const auto& [path_comp, child] : this->children) {
        auto path_comp_fb = builder.CreateString(path_comp);

        auto serialized_child = child->serialize(builder);

        children.push_back(slamd::flatb::CreateChildEntry(
            builder,
            path_comp_fb,
            serialized_child
        ));
    }

    auto children_fb = builder.CreateVector(children);

    auto maybe_geom = this->get_object();
    std::optional<uint64_t> maybe_geom_id_fb = std::nullopt;

    if (maybe_geom.has_value()) {
        maybe_geom_id_fb = maybe_geom.value()->id.value;
    }

    slamd::flatb::NodeBuilder node_builder(builder);
    if (maybe_geom_id_fb.has_value()) {
        node_builder.add_geometry_id(maybe_geom_id_fb.value());
    }

    auto maybe_transform = this->get_transform();
    if (maybe_transform.has_value()) {
        slamd::flatb::Mat4 transform_fb =
            gmath::serialize(maybe_transform.value());
        node_builder.add_transform(&transform_fb);
    }

    node_builder.add_children(children_fb);

    return node_builder.Finish();
}

void Node::set_transform(
    glm::mat4 transform
) {
    std::scoped_lock l(this->transform_mutex);
    this->transform = transform;

    flatbuffers::FlatBufferBuilder builder;

    auto path_fb = builder.CreateString(this->path.string());
    auto transform_fb = gmath::serialize(transform);
    ;

    auto set_transform_fb = flatb::CreateSetTransform(
        builder,
        this->tree->id.value,
        path_fb,
        &transform_fb
    );

    auto message_fb = flatb::CreateMessage(
        builder,
        flatb::MessageUnion_set_transform,
        set_transform_fb.Union()
    );

    builder.Finish(message_fb);

    auto message_buffer = _utils::builder_buffer(builder);
    this->broadcast(message_buffer);
}

void Node::broadcast(
    std::shared_ptr<std::vector<uint8_t>> message_buffer
) {
    this->tree->broadcast(message_buffer);
}

Tree::Tree()
    : id(_id::TreeID::next()) {
    this->root = std::make_shared<Node>(this, TreePath());
}

void Tree::broadcast(
    std::shared_ptr<std::vector<uint8_t>> message_buffer
) {
    for (auto it = this->attached_to.begin(); it != this->attached_to.end();) {
        if (auto view = it->second.lock()) {
            view->broadcast(message_buffer);
            it++;
        } else {
            it = attached_to.erase(it);
        }
    }
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
    return slamd::flatb::CreateTree(builder, this->id.value, serialized_root);
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
    TreePath curr_path;

    for (size_t i = 0; i < path.components.size(); i++) {
        auto& component = path.components[i];
        curr_path = curr_path / component;

        auto child_iterator = current_node->children.find(component);

        if (child_iterator == current_node->children.end()) {
            // in this case, we want to create the path down to the target
            // node

            auto new_node = std::make_shared<Node>(this, curr_path);

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
    // _global::trees.add(scene->id, scene);
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
    // _global::trees.add(canvas->id, canvas);
    return canvas;
}

}  // namespace slamd
#include <slamd/tree/node.hpp>
#include <slamd_common/gmath/serialization.hpp>

namespace slamd {
namespace _tree {

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
    this->object.emplace(object);
}

flatbuffers::Offset<slamd::flatb::Node> Node::serialize(
    flatbuffers::FlatBufferBuilder& builder
) {
    std::vector<flatbuffers::Offset<slamd::flatb::ChildEntry>> children;

    for (const auto& [path_comp, child] : this->children) {
        auto path_comp_fb = builder.CreateString(path_comp);

        auto serialized_child = child->serialize(builder);

        children.push_back(
            slamd::flatb::CreateChildEntry(
                builder,
                path_comp_fb,
                serialized_child
            )
        );
    }

    auto children_fb = builder.CreateVector(children);

    auto maybe_geom = this->get_object();
    std::optional<flatbuffers::Offset<flatb::Geometry>> maybe_geom_fb =
        std::nullopt;
    if (maybe_geom.has_value()) {
        maybe_geom_fb = maybe_geom.value()->serialize(builder);
    }

    slamd::flatb::NodeBuilder node_builder(builder);
    if (maybe_geom_fb.has_value()) {
        node_builder.add_geometry(maybe_geom_fb.value());
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
}

}  // namespace _tree
}  // namespace slamd
#include <slamd/tree/node.hpp>
#include <stdexcept>

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

    slamd::flatb::Mat4 * transform_ptr = nullptr;
    auto maybe_transform = this->get_transform();
    // if(maybe_transform.has_value()) {


    //     slamd::flatb::Mat4 transform_fb = {maybe_transform.value()};

    //     transform_fb = builder.CreateStruct(

    //     )
    // }

    // slamd::flatb::CreateNode(
    //     builder,
    //     children_fb,
    // )
}

void Node::set_transform(
    glm::mat4 transform
) {
    std::scoped_lock l(this->transform_mutex);
    this->transform = transform;
}

}  // namespace _tree
}  // namespace slamd
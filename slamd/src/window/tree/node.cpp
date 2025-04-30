#include <spdlog/spdlog.h>
#include <slamd_common/gmath/serialization.hpp>
#include <slamd_common/gmath/stringify.hpp>
#include <slamd_window/geom/geometry.hpp>
#include <slamd_window/tree/node.hpp>

namespace slamd {

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

void Node::set_transform(
    glm::mat4 transform
) {
    std::scoped_lock l(this->transform_mutex);
    this->transform = transform;
}

std::unique_ptr<Node> Node::deserialize(
    const slamd::flatb::Node* node_fb,
    std::map<slamd::_id::GeometryID, std::shared_ptr<_geom::Geometry>>&
        geometries
) {
    std::unique_ptr<Node> node = std::make_unique<Node>();

    auto transform_fb = node_fb->transform();
    if (transform_fb != nullptr) {
        glm::mat4 transform = slamd::gmath::deserialize(transform_fb);

        spdlog::debug("Found transform {}", slamd::gmath::stringify(transform));

        node->set_transform(transform);
    }

    auto geom_id_fb = node_fb->geometry_id();
    if (geom_id_fb != 0) {
        node->set_object(geometries[_id::GeometryID(geom_id_fb)]);
    }

    auto children_fb = node_fb->children();
    spdlog::debug("Found {} children", children_fb->size());

    for (auto child_fb : *children_fb) {
        std::string key = child_fb->key()->str();
        auto child = Node::deserialize(child_fb->value(), geometries);

        node->children.insert({key, std::move(child)});
    }

    return node;
}

}  // namespace slamd
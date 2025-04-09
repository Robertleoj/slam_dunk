#include <slamd/scene_node.hpp>
#include <stdexcept>

namespace slamd {

std::optional<std::shared_ptr<geometry::Geometry>> ObjectReference::get_object(
) const {
    switch (tag) {
        case WEAK: {
            return this->weak_obj.lock();
        }
        case STRONG: {
            return this->strong_obj;
        }
        default: {
            throw std::runtime_error("Invalid tag");
        }
    }
}

void SceneNode::set_object(
    std::shared_ptr<geometry::Geometry> object
) {
    std::scoped_lock l(this->object_mutex);
    this->object_reference.emplace(object);
}

void SceneNode::set_object(
    std::weak_ptr<geometry::Geometry> object
) {
    std::scoped_lock l(this->object_mutex);
    this->object_reference.emplace(object);
}

void SceneNode::set_transform(
    glm::mat4 transform
) {
    std::scoped_lock l(this->transform_mutex);
    this->transform = transform;
}

std::optional<std::shared_ptr<geometry::Geometry>> SceneNode::get_object(
) const {
    std::scoped_lock l(this->object_mutex);
    if (!this->object_reference.has_value()) {
        return std::nullopt;
    }

    return this->object_reference.value().get_object();
}

std::optional<glm::mat4> SceneNode::get_transform() const {
    std::scoped_lock l(this->transform_mutex);
    return this->transform;
}

}  // namespace slamd
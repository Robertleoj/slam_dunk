#include <slamd/tree/node.hpp>
#include <stdexcept>

namespace slamd {
namespace _tree {

ObjectReference::ObjectReference(
    const std::shared_ptr<_geometry::Geometry>& obj
)
    : tag(STRONG),
      strong_obj(obj) {}
ObjectReference::ObjectReference(
    const std::weak_ptr<_geometry::Geometry>& obj
)
    : tag(WEAK),
      weak_obj(obj) {}
ObjectReference::~ObjectReference() {
    switch (tag) {
        case WEAK: {
            weak_obj.~weak_ptr<_geometry::Geometry>();
        }
        case STRONG: {
            strong_obj.~shared_ptr<_geometry::Geometry>();
        }
    }
}

std::optional<std::shared_ptr<_geometry::Geometry>> ObjectReference::get_object(
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

std::optional<std::shared_ptr<_geometry::Geometry>> Node::get_object() const {
    std::scoped_lock l(this->object_mutex);
    if (!this->object_reference.has_value()) {
        return std::nullopt;
    }

    return this->object_reference.value().get_object();
}

std::optional<glm::mat4> Node::get_transform() const {
    std::scoped_lock l(this->transform_mutex);
    return this->transform;
}

void Node::set_object(
    std::shared_ptr<_geometry::Geometry> object
) {
    std::scoped_lock l(this->object_mutex);
    this->object_reference.emplace(object);
}

void Node::set_object(
    std::weak_ptr<_geometry::Geometry> object
) {
    std::scoped_lock l(this->object_mutex);
    this->object_reference.emplace(object);
}

void Node::set_transform(
    glm::mat4 transform
) {
    std::scoped_lock l(this->transform_mutex);
    this->transform = transform;
}

}  // namespace _tree
}  // namespace slamd
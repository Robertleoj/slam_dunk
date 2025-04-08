#include <slamd/node.hpp>

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

void Node::set_object(
    std::shared_ptr<geometry::Geometry> object
) {
    this->object_reference.emplace(object);
}

void Node::set_object(
    std::weak_ptr<geometry::Geometry> object
) {
    this->object_reference.emplace(object);
}

void Node::set_transform(
    glm::mat4 transform
) {
    this->transform = transform;
}

std::optional<std::shared_ptr<geometry::Geometry>> Node::get_object() const {
    if (!this->object_reference.has_value()) {
        return std::nullopt;
    }

    return this->object_reference.value().get_object();
}

}  // namespace slamd
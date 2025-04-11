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

void Node::set_transform(
    glm::mat4 transform
) {
    std::scoped_lock l(this->transform_mutex);
    this->transform = transform;
}

}  // namespace _tree
}  // namespace slamd
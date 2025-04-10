#pragma once
#include <glm/glm.hpp>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <slamd/geometry/geometry.hpp>
#include <string>

namespace slamd {
namespace _tree {

class Node {
   public:
    std::map<std::string, std::unique_ptr<Node>> children;

   private:
    std::optional<glm::mat4> transform;
    std::optional<std::shared_ptr<_geometry::Geometry>> object;

    mutable std::mutex transform_mutex;
    mutable std::mutex object_mutex;

   public:
    std::optional<std::shared_ptr<_geometry::Geometry>> get_object() const;

    std::optional<glm::mat4> get_transform() const;

    void set_object(std::shared_ptr<_geometry::Geometry> object);

    void set_transform(glm::mat4 transform);
};
}  // namespace _tree

}  // namespace slamd
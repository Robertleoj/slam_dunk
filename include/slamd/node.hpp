#pragma once
#include <glm/glm.hpp>
#include <map>
#include <memory>
#include <optional>
#include <slamd/geometry/geometry.hpp>
#include <string>

namespace slamd {

class ObjectReference {
   public:
    ObjectReference(
        const std::shared_ptr<geometry::Geometry>& obj
    )
        : tag(STRONG),
          strong_obj(obj) {}

    ObjectReference(
        const std::weak_ptr<geometry::Geometry>& obj
    )
        : tag(WEAK),
          weak_obj(obj) {}

    ~ObjectReference() {
        switch (tag) {
            case WEAK: {
                weak_obj.~weak_ptr<geometry::Geometry>();
            }
            case STRONG: {
                strong_obj.~shared_ptr<geometry::Geometry>();
            }
        }
    }

    std::optional<std::shared_ptr<geometry::Geometry>> get_object() const;

   private:
    enum {
        STRONG,
        WEAK
    } tag;

    union {
        std::weak_ptr<geometry::Geometry> weak_obj;
        std::shared_ptr<geometry::Geometry> strong_obj;
    };
};

class Node {
   public:
    std::map<std::string, std::unique_ptr<Node>> children;
    std::optional<glm::mat4> transform;

   private:
    std::optional<ObjectReference> object_reference;

   public:
    std::optional<std::shared_ptr<geometry::Geometry>> get_object() const;
    void set_object(std::shared_ptr<geometry::Geometry> object);
    void set_object(std::weak_ptr<geometry::Geometry> object);
    void set_transform(glm::mat4 transform);
};

}  // namespace slamd
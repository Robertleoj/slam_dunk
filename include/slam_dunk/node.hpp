#pragma once
#include <glm/glm.hpp>
#include <map>
#include <memory>
#include <optional>
#include <slam_dunk/scene_object.hpp>
#include <string>

namespace sdunk {

class ObjectReference {
   public:
    ObjectReference(
        const std::shared_ptr<SceneObject>& obj
    )
        : tag(STRONG),
          strong_obj(obj) {}

    ObjectReference(
        const std::weak_ptr<SceneObject>& obj
    )
        : tag(WEAK),
          weak_obj(obj) {}

    ~ObjectReference() {
        switch (tag) {
            case WEAK: {
                weak_obj.~weak_ptr<SceneObject>();
            }
            case STRONG: {
                strong_obj.~shared_ptr<SceneObject>();
            }
        }
    }

    std::optional<std::shared_ptr<SceneObject>> get_object();

   private:
    enum {
        STRONG,
        WEAK
    } tag;

    union {
        std::weak_ptr<SceneObject> weak_obj;
        std::shared_ptr<SceneObject> strong_obj;
    };
};

class Node {
   public:
    std::map<std::string, std::unique_ptr<Node>> children;
    std::optional<glm::mat4> transform;

   private:
    std::optional<ObjectReference> object_reference;

   public:
    std::optional<std::shared_ptr<SceneObject>> get_object();
    void set_object(std::shared_ptr<SceneObject> object);
    void set_object(std::weak_ptr<SceneObject> object);
    void set_transform(glm::mat4 transform);
};

}  // namespace sdunk
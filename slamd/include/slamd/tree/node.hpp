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

template <typename D>
class ObjectReference {
   public:
    ObjectReference(
        const std::shared_ptr<D>& obj
    )
        : tag(STRONG),
          strong_obj(obj) {}

    ObjectReference(
        const std::weak_ptr<D>& obj
    )
        : tag(WEAK),
          weak_obj(obj) {}

    ~ObjectReference() {
        switch (tag) {
            case WEAK: {
                weak_obj.~weak_ptr<D>();
            }
            case STRONG: {
                strong_obj.~shared_ptr<D>();
            }
        }
    }

    std::optional<std::shared_ptr<D>> get_object() const {
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

   private:
    enum {
        STRONG,
        WEAK
    } tag;

    union {
        std::weak_ptr<D> weak_obj;
        std::shared_ptr<D> strong_obj;
    };
};

template <typename D, typename T>
class Node {
   public:
    std::map<std::string, std::unique_ptr<Node>> children;

   private:
    std::optional<T> transform;
    std::optional<ObjectReference<D>> object_reference;

    mutable std::mutex transform_mutex;
    mutable std::mutex object_mutex;

   public:
    std::optional<std::shared_ptr<D>> get_object() const {
        std::scoped_lock l(this->object_mutex);
        if (!this->object_reference.has_value()) {
            return std::nullopt;
        }

        return this->object_reference.value().get_object();
    }

    std::optional<T> get_transform() const {
        std::scoped_lock l(this->transform_mutex);
        return this->transform;
    }

    void set_object(
        std::shared_ptr<D> object
    ) {
        std::scoped_lock l(this->object_mutex);
        this->object_reference.emplace(object);
    }

    void set_object(
        std::weak_ptr<D> object
    ) {
        std::scoped_lock l(this->object_mutex);
        this->object_reference.emplace(object);
    }

    void set_transform(
        T transform
    ) {
        std::scoped_lock l(this->transform_mutex);
        this->transform = transform;
    }
};
}  // namespace _tree

}  // namespace slamd
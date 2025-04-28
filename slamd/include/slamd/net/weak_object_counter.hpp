#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

namespace slamd {
namespace _net {

template <typename T>
class WeakObjectCounter {
   public:
    void add(
        std::shared_ptr<T> obj
    ) {
        cleanup();  // optional, keeps things tidy
        ++objects[obj];
    }

    void remove(
        std::shared_ptr<T> obj
    ) {
        cleanup();
        auto it = objects.find(obj);
        if (it != objects.end()) {
            if (--(it->second) == 0) {
                objects.erase(it);
            }
        }
    }

    bool contains(
        std::shared_ptr<T> obj
    ) {
        cleanup();
        return objects.find(obj) != objects.end();
    }

    std::vector<std::shared_ptr<T>> alive_objects() {
        cleanup();
        std::vector<std::shared_ptr<T>> result;
        for (const auto& [weak_obj, count] : objects) {
            if (auto sp = weak_obj.lock()) {
                result.push_back(sp);
            }
        }
        return result;
    }

   private:
    struct WeakPtrHash {
        std::size_t operator()(
            const std::weak_ptr<T>& wp
        ) const {
            auto sp = wp.lock();
            return std::hash<T*>()(sp.get());
        }
    };

    struct WeakPtrEqual {
        bool operator()(
            const std::weak_ptr<T>& a,
            const std::weak_ptr<T>& b
        ) const {
            return a.lock() == b.lock();
        }
    };

    void cleanup() {
        for (auto it = objects.begin(); it != objects.end();) {
            if (it->first.expired()) {
                it = objects.erase(it);
            } else {
                ++it;
            }
        }
    }

    std::unordered_map<std::weak_ptr<T>, int, WeakPtrHash, WeakPtrEqual>
        objects;
};

}  // namespace _net
}  // namespace slamd
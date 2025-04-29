#pragma once

#include <memory>
#include <mutex>
#include <optional>
#include <slamd/geom/geometry.hpp>
#include <slamd/tree/tree.hpp>
#include <slamd/view.hpp>
#include <slamd/visualizer.hpp>
#include <unordered_map>

namespace slamd {
namespace _global {

template <typename IDType, typename T>
class ObjectTracker {
   public:
    void add(
        std::shared_ptr<T> obj
    ) {
        std::lock_guard<std::mutex> lock(this->mutex);
        this->map[obj->id] = obj;
    }

    std::optional<std::shared_ptr<T>> get(
        IDType id
    ) {
        std::lock_guard<std::mutex> lock(this->mutex);

        auto it = this->map.find(id);
        if (it == this->map.end()) {
            return std::nullopt;
        }

        auto ptr = it->second.lock();
        if (!ptr) {
            this->map.erase(it);
            return std::nullopt;
        }

        return ptr;
    }

    void remove(
        IDType id
    ) {
        std::lock_guard<std::mutex> lock(this->mutex);
        map.erase(id);
    }

   private:
    std::unordered_map<IDType, std::weak_ptr<T>> map;
    std::mutex mutex;
};

extern ObjectTracker<_id::NodeID, _tree::Node> nodes;
extern ObjectTracker<_id::ViewID, _view::View> views;

}  // namespace _global
}  // namespace slamd
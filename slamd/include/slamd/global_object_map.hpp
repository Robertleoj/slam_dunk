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

template <typename T>
class GlobalObjectMap {
   public:
    void add(
        uint64_t id,
        std::shared_ptr<T> obj
    ) {
        std::lock_guard<std::mutex> lock(this->mutex);
        this->map[id] = obj;
    }

    std::optional<std::shared_ptr<T>> get(
        uint64_t id
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
        uint64_t id
    ) {
        std::lock_guard<std::mutex> lock(this->mutex);
        map.erase(id);
    }

   private:
    std::unordered_map<uint64_t, std::weak_ptr<T>> map;
    std::mutex mutex;
};

extern GlobalObjectMap<Visualizer> visualizers;
extern GlobalObjectMap<_tree::Tree> trees;
extern GlobalObjectMap<View> views;
extern GlobalObjectMap<_geom::Geometry> geometries;

}  // namespace _global
}  // namespace slamd
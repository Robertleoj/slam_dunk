#pragma once
#include <flatb/visualizer_generated.h>
#include <glm/glm.hpp>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <slamd/geom/geometry.hpp>
#include <string>
namespace slamd {
namespace _tree {

class Node {
   public:
    std::map<std::string, std::unique_ptr<Node>> children;

   private:
    std::optional<glm::mat4> transform;
    std::optional<std::shared_ptr<_geom::Geometry>> object;

    mutable std::mutex transform_mutex;
    mutable std::mutex object_mutex;

   public:
    std::optional<std::shared_ptr<_geom::Geometry>> get_object() const;

    flatbuffers::Offset<slamd::flatb::Node> serialize(
        flatbuffers::FlatBufferBuilder& builder
    );

    std::optional<glm::mat4> get_transform() const;

    void set_object(std::shared_ptr<_geom::Geometry> object);

    void set_transform(glm::mat4 transform);
};
}  // namespace _tree

}  // namespace slamd
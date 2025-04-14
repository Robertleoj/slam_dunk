#pragma once
#include <slamd/data/mesh.hpp>
#include <slamd/geom/geometry.hpp>
#include <slamd/geom/mono_instanced.hpp>

namespace slamd {
namespace _geom {

/**
 * TODO: implement like pointcloud
 * to allow updating nicely
 */

class Circles2D : public Geometry {
   public:
    Circles2D(
        const std::vector<glm::vec3>& positions,
        const std::vector<glm::vec3>& colors,
        const std::vector<float>& radii,
        float thickness
    );

    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

    std::optional<gmath::AABB> bounds() override;

   private:
    static data::MeshData make_instance_mesh();

    static MonoInstanced make_mono_instanced(
        const std::vector<glm::vec3>& positions,
        const std::vector<glm::vec3>& colors,
        const std::vector<float>& radii
    );

    static gmath::AABB make_bounds(
        const std::vector<glm::vec3>& positions,
        const std::vector<float>& radii
    );

   private:
    MonoInstanced circles_instanced;
    gmath::AABB cached_bounds;

    std::vector<glm::vec3> positions;
    std::vector<float> radii;
};

}  // namespace _geom
}  // namespace slamd
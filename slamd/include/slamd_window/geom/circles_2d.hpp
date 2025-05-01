#pragma once
#include <slamd_common/data/mesh.hpp>
#include <slamd_window/geom/geometry.hpp>
#include <slamd_window/geom/mono_instanced.hpp>

namespace slamd {
namespace _geom {

class Circles2D : public Geometry {
   public:
    Circles2D(
        const std::vector<glm::vec2>& positions,
        const std::vector<glm::vec3>& colors,
        const std::vector<float>& radii,
        float thickness
    );

    static std::shared_ptr<Circles2D> deserialize(
        const slamd::flatb::Circles2D* triad_fb
    );

    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

    std::optional<slamd::gmath::AABB> bounds() override;

    void update_positions(const std::vector<glm::vec2>& positions);
    void update_colors(const std::vector<glm::vec3>& colors);
    void update_radii(const std::vector<float>& radii);

   private:
    static std::unique_ptr<MonoInstanced> make_mono_instanced(
        const std::vector<glm::vec2>& positions,
        const std::vector<glm::vec3>& colors,
        const std::vector<float>& radii,
        float thickness
    );

    static slamd::gmath::AABB make_bounds(
        const std::vector<glm::vec2>& positions,
        const std::vector<float>& radii
    );

    static std::vector<glm::mat4>
    make_transforms(std::vector<glm::vec2> positions, std::vector<float> radii);

    void handle_updates();

   private:
    std::unique_ptr<MonoInstanced> circles_instanced;
    slamd::gmath::AABB cached_bounds;

    std::vector<glm::vec3> colors;
    std::vector<glm::vec2> positions;
    std::vector<float> radii;

    bool pending_trans_update = false;
    bool pending_color_update = false;
};

}  // namespace _geom

namespace geom2d {
using CirclesPtr = std::shared_ptr<_geom::Circles2D>;

CirclesPtr circles(
    const std::vector<glm::vec2>& positions,
    const std::vector<glm::vec3>& colors,
    const std::vector<float>& radii,
    float thickness = 0.1
);
}  // namespace geom2d
}  // namespace slamd
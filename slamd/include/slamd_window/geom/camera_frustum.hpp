#pragma once
#include <slamd_common/data/image.hpp>
#include <slamd_window/geom/geometry.hpp>
#include <slamd_window/geom/image.hpp>
#include <slamd_window/geom/poly_line.hpp>

namespace slamd {
namespace _geom {

class CameraFrustum : public Geometry {
   public:
    CameraFrustum(
        glm::mat3 intrinsics_matrix,
        size_t image_width,
        size_t image_height,
        float scale = 1.0
    );

    CameraFrustum(
        glm::mat3 intrinsics_matrix,
        size_t image_width,
        size_t image_height,
        slamd::data::Image&& image,
        float scale = 1.0
    );

    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

    static std::shared_ptr<CameraFrustum> deserialize(
        const slamd::flatb::CameraFrustum* frustum_fb
    );

   private:
    struct Corners {
        glm::vec3 tl;
        glm::vec3 tr;
        glm::vec3 br;
        glm::vec3 bl;
    };

    Corners corners;

    struct CameraImage {
        std::unique_ptr<Image> image_geometry;
        // transform to put the image into the frustum
        glm::mat4 transform;
    };
    std::vector<std::unique_ptr<PolyLine>> poly_lines;

    glm::mat4 scale_transform;
    std::optional<CameraImage> cam_image;
};
}  // namespace _geom
}  // namespace slamd
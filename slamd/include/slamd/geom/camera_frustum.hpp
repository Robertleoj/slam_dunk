#pragma once
#include <slamd/geom/geometry.hpp>
#include <slamd/geom/image.hpp>
#include <slamd/geom/poly_line.hpp>

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
        data::Image&& image,
        float scale = 1.0
    );

    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

   private:
    struct Corners {
        glm::vec3 tl;
        glm::vec3 tr;
        glm::vec3 br;
        glm::vec3 bl;
    };

    Corners corners;

    struct CameraImage {
        Image image_geometry;
        // transform to put the image into the frustum
        glm::mat4 transform;
    };

    glm::mat4 scale_transform;
    std::optional<CameraImage> cam_image;
    std::vector<PolyLine> poly_lines;
};
}  // namespace _geom

namespace geom {

using _geom::CameraFrustum;

std::shared_ptr<CameraFrustum> camera_frustum(
    glm::mat3 intrinsics_matrix,
    size_t image_width,
    size_t image_height,
    data::Image&& image,
    float scale = 1.0
);

std::shared_ptr<CameraFrustum> camera_frustum(
    glm::mat3 intrinsics_matrix,
    size_t image_width,
    size_t image_height,
    float scale = 1.0
);

}  // namespace geom
}  // namespace slamd
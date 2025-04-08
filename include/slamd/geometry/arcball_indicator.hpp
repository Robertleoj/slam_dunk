#pragma once

#include <chrono>
#include <slamd/geometry/geometry.hpp>
#include <slamd/shaders.hpp>

namespace slamd {
namespace geometry {

class ArcballIndicator : public Geometry {
   public:
    ArcballIndicator();
    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

    void set_arcball_zoom(float zoom);
    void interact();

   private:
    static glm::mat4 get_scale_mat(float scale);
    float get_alpha();

   private:
    gl::GLuint vao_id;
    gl::GLuint vbo_id;
    uint vertex_count;
    ShaderProgram shader;
    float arcball_zoom;
    std::optional<std::chrono::high_resolution_clock::time_point>
        last_interacted;
};

}  // namespace geometry
}  // namespace slamd
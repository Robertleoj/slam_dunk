#pragma once

#include <chrono>
#include <slamd/geometry/geometry.hpp>
#include <slamd/shaders.hpp>
#include <slamd/thread_box.hpp>

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
    void initialize();

   private:
    struct GLState {
        gl::GLuint vao_id;
        gl::GLuint vbo_id;
        ShaderProgram shader;
    };
    std::optional<ThreadBox<GLState>> gl_state;
    std::optional<std::thread::id> render_thread_id;

    uint vertex_count;
    float arcball_zoom;
    std::optional<std::chrono::high_resolution_clock::time_point>
        last_interacted;
};

std::shared_ptr<ArcballIndicator> arcball_indicator();

}  // namespace geometry
}  // namespace slamd
#pragma once

#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <slamd/geom/geometry.hpp>
#include <slamd/shaders.hpp>
#include <slamd/thread_box.hpp>
#include <vector>

namespace slamd {
namespace _geom {

class GridXYPlane : public Geometry {
   public:
    GridXYPlane(float grid_size = 10.0f);

    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

    ~GridXYPlane() override;

    void set_arcball_zoom(float zoom);

   private:
    void initialize();

   private:
    struct GLData {
        gl::GLuint vao_id;
        gl::GLuint vbo_id;
        ShaderProgram shader;
        size_t vertex_count;
    };

    std::optional<ThreadBox<GLData>> gl_data;
    std::optional<std::thread::id> render_thread_id;

    float arcball_zoom;
    float grid_size;
};

}  // namespace _geom
}  // namespace slamd
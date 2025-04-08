#pragma once

#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <slamd/geometry/geometry.hpp>
#include <slamd/shaders.hpp>
#include <vector>

namespace slamd {
namespace geometry {

class GridXYPlane : public Geometry {
   public:
    GridXYPlane(float grid_size = 10.0f);

    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

    ~GridXYPlane() override;

    void set_arcball_zoom(float zoom);

   private:
    gl::GLuint vao_id;
    gl::GLuint vbo_id;
    int vertex_count;
    ShaderProgram shader;
    float arcball_zoom;
};

}  // namespace geometry
}  // namespace slamd
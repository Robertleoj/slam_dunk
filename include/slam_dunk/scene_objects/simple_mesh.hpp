#pragma once
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <glm/glm.hpp>
#include <slam_dunk/scene_object.hpp>
#include <vector>

namespace sdunk {

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
};

class SimpleMesh : public SceneObject {
   public:
    SimpleMesh(
        std::vector<Vertex> vertices,
        std::vector<uint32_t> triangle_indices
    );

    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

   private:
    gl::GLuint vao_id;
    gl::GLuint vbo_id;
    gl::GLuint eab_id;

    size_t num_vertices;
};
}  // namespace sdunk
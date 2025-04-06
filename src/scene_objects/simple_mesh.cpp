#include <slam_dunk/scene_objects/simple_mesh.hpp>

namespace sdunk {
SimpleMesh::SimpleMesh(
    std::vector<Vertex> vertices,
    std::vector<uint32_t> triangle_indices
) {
    // create the vertex array object
    gl::glGenVertexArrays(1, &this->vao_id);
    gl::glBindVertexArray(this->vao_id);

    // make the vertex buffer object
    gl::glGenBuffers(1, &this->vbo_id);
    gl::glBindBuffer(gl::GL_ARRAY_BUFFER, this->vbo_id);
    gl::glBufferData(
        gl::GL_ARRAY_BUFFER,
        vertices.size() * sizeof(Vertex),
        vertices.data(),
        gl::GL_STATIC_DRAW
    );

    // make the element array buffer
    gl::glGenBuffers(1, &this->eab_id);
    gl::glBindBuffer(gl::GL_ELEMENT_ARRAY_BUFFER, this->eab_id);

    gl::glBufferData(
        gl::GL_ELEMENT_ARRAY_BUFFER,
        triangle_indices.size() * sizeof(uint32_t),
        triangle_indices.data(),
        gl::GL_STATIC_DRAW
    );

    // Position attribute (location = 0)
    gl::glVertexAttribPointer(
        0,
        3,
        gl::GL_FLOAT,
        gl::GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, position)
    );
    gl::glEnableVertexAttribArray(0);

    // Color attribute (location = 1)
    glVertexAttribPointer(
        1,
        3,
        gl::GL_FLOAT,
        gl::GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, color)
    );
    gl::glEnableVertexAttribArray(1);

    // unbind the vao
    gl::glBindVertexArray(0);
}

void SimpleMesh::render(
    glm::mat4 model,
    glm::mat4 view,
    glm::mat4 projection
) {
    gl::glBindVertexArray(this->vao_id);
    gl::glDrawElements(gl::GL_TRIANGLES, 6, gl::GL_UNSIGNED_INT, 0);
    gl::glBindVertexArray(0);
};
}  // namespace sdunk
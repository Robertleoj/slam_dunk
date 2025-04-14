#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <format>
#include <ranges>
#include <slamd/assert.hpp>
#include <slamd/constants.hpp>
#include <slamd/gen/shader_sources.hpp>
#include <slamd/geom/mono_instanced.hpp>
#include <slamd/geom/utils.hpp>

namespace slamd {
namespace _geom {

MonoInstanced::MonoInstanced(
    const data::MeshData& instance_mesh,
    const std::vector<glm::mat4>& transforms,
    const std::vector<glm::vec3>& colors
)
    : mesh_data(instance_mesh),
      transforms(transforms),
      pending_trans_update(false),
      colors(colors),
      pending_colors_update(false) {
    if (!((transforms.size() == colors.size()))) {
        throw std::invalid_argument(
            std::format(
                "number of transforms, and colors got "
                "{} transforms and {} colors",
                transforms.size(),
                colors.size()
            )
        );
    }
}

std::tuple<uint, uint> MonoInstanced::initialize_mesh() {
    // vertex buffer
    uint mesh_vbo_id;
    gl::glGenBuffers(1, &mesh_vbo_id);
    gl::glBindBuffer(gl::GL_ARRAY_BUFFER, mesh_vbo_id);

    size_t vert_size = this->mesh_data.positions.size() * sizeof(glm::vec3);
    size_t normals_size = this->mesh_data.normals.size() * sizeof(glm::vec3);

    gl::glBufferData(
        gl::GL_ARRAY_BUFFER,
        vert_size + normals_size,
        nullptr,
        gl::GL_STATIC_DRAW
    );
    gl::glBufferSubData(
        gl::GL_ARRAY_BUFFER,
        0,
        vert_size,
        this->mesh_data.positions.data()
    );
    gl::glBufferSubData(
        gl::GL_ARRAY_BUFFER,
        vert_size,
        normals_size,
        this->mesh_data.normals.data()
    );

    // element buffer
    uint mesh_eab_id;
    gl::glGenBuffers(1, &mesh_eab_id);
    gl::glBindBuffer(gl::GL_ELEMENT_ARRAY_BUFFER, mesh_eab_id);

    gl::glBufferData(
        gl::GL_ELEMENT_ARRAY_BUFFER,
        this->mesh_data.triangle_indices.size() * sizeof(uint32_t),
        this->mesh_data.triangle_indices.data(),
        gl::GL_STATIC_DRAW
    );

    // and this is the base vertex attribute
    gl::glVertexAttribPointer(
        0,
        3,
        gl::GL_FLOAT,
        gl::GL_FALSE,
        sizeof(glm::vec3),
        (void*)0
    );
    gl::glEnableVertexAttribArray(0);

    // then we have the normal attribute
    gl::glVertexAttribPointer(
        1,
        3,
        gl::GL_FLOAT,
        gl::GL_FALSE,
        sizeof(glm::vec3),
        (void*)vert_size
    );
    gl::glEnableVertexAttribArray(1);

    return std::make_tuple(mesh_vbo_id, mesh_eab_id);
}

uint MonoInstanced::initialize_trans_buffer() {
    uint trans_vbo_id;
    gl::glGenBuffers(1, &trans_vbo_id);

    gl::glBindBuffer(gl::GL_ARRAY_BUFFER, trans_vbo_id);

    gl::glBufferData(
        gl::GL_ARRAY_BUFFER,
        this->transforms.size() * sizeof(glm::mat4),
        this->transforms.data(),
        gl::GL_DYNAMIC_DRAW
    );

    gl::glVertexAttribPointer(
        2,
        3,
        gl::GL_FLOAT,
        gl::GL_FALSE,
        sizeof(glm::mat4),
        (void*)0
    );
    gl::glEnableVertexAttribArray(2);
    gl::glVertexAttribDivisor(2, 1);

    return trans_vbo_id;
}

uint MonoInstanced::initialize_color_buffer() {
    uint vbo_id;
    gl::glGenBuffers(1, &vbo_id);

    gl::glBindBuffer(gl::GL_ARRAY_BUFFER, vbo_id);

    gl::glBufferData(
        gl::GL_ARRAY_BUFFER,
        this->colors.size() * sizeof(glm::vec3),
        this->colors.data(),
        gl::GL_DYNAMIC_DRAW
    );

    gl::glVertexAttribPointer(
        3,
        3,
        gl::GL_FLOAT,
        gl::GL_FALSE,
        sizeof(glm::vec3),
        (void*)0
    );
    gl::glEnableVertexAttribArray(4);
    // one per instance
    gl::glVertexAttribDivisor(4, 1);

    return vbo_id;
}

void MonoInstanced::maybe_initialize() {
    if (this->render_thread_id.has_value()) {
        assert_thread(this->render_thread_id.value());
        return;
    }

    this->render_thread_id = std::this_thread::get_id();

    uint vao_id;
    gl::glGenVertexArrays(1, &vao_id);
    gl::glBindVertexArray(vao_id);

    auto [mesh_vbo_id, mesh_eab_id] = this->initialize_mesh();
    uint trans_vbo_id = this->initialize_trans_buffer();
    uint colors_vbo_id = this->initialize_color_buffer();

    gl::glBindVertexArray(0);

    this->gl_data.emplace(GLData(
        ShaderProgram(
            shader_source::mono_instanced::vert,
            shader_source::mono_instanced::frag
        ),
        vao_id,
        mesh_vbo_id,
        mesh_eab_id,
        trans_vbo_id,
        colors_vbo_id
    ));
}

void MonoInstanced::update_transforms(
    const std::vector<glm::mat4>& transforms
) {
    this->transforms = transforms;
    this->pending_trans_update = true;
}

void MonoInstanced::update_colors(
    const std::vector<glm::vec3>& colors
) {
    this->colors = colors;
    this->pending_colors_update = true;
}

void MonoInstanced::handle_updates() {
    if (this->pending_trans_update) {
        gl::glBindBuffer(
            gl::GL_ARRAY_BUFFER,
            this->gl_data.value().trans_vbo_id
        );
        gl::glBufferData(
            gl::GL_ARRAY_BUFFER,
            this->transforms.size() * sizeof(glm::mat4),
            this->transforms.data(),
            gl::GL_DYNAMIC_DRAW
        );
        this->pending_trans_update = false;
    }

    if (this->pending_colors_update) {
        gl::glBindBuffer(
            gl::GL_ARRAY_BUFFER,
            this->gl_data.value().colors_vbo_id
        );
        gl::glBufferData(
            gl::GL_ARRAY_BUFFER,
            this->colors.size() * sizeof(glm::vec3),
            this->colors.data(),
            gl::GL_DYNAMIC_DRAW
        );
        this->pending_colors_update = false;
    }
}

void MonoInstanced::render(
    glm::mat4 model,
    glm::mat4 view,
    glm::mat4 projection
) {
    this->maybe_initialize();

    auto& gl_data = this->gl_data.value();

    gl::glBindVertexArray(gl_data.vao_id);
    this->handle_updates();

    gl_data.shader.use();

    gl_data.shader.set_uniform("u_model", model);
    gl_data.shader.set_uniform("u_view", view);
    gl_data.shader.set_uniform("u_projection", projection);
    gl_data.shader.set_uniform("u_light_dir", slamd::_const::light_dir);
    gl_data.shader.set_uniform(
        "u_min_brightness",
        _const::default_min_brightness
    );
    gl::glDrawElementsInstanced(
        gl::GL_TRIANGLES,
        this->mesh_data.triangle_indices.size(),
        gl::GL_UNSIGNED_INT,
        0,
        this->transforms.size()
    );

    gl::glBindVertexArray(0);
}

}  // namespace _geom
}  // namespace slamd
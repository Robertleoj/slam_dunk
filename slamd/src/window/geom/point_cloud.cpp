#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <format>
#include <ranges>
#include <slamd/assert.hpp>
#include <slamd/constants.hpp>
#include <slamd/gen/shader_sources.hpp>
#include <slamd/geom/point_cloud.hpp>
#include <slamd/geom/utils.hpp>

namespace slamd {
namespace _geom {

PointCloud::PointCloud(
    const std::vector<glm::vec3>& positions,
    const std::vector<glm::vec3>& colors,
    const std::vector<float>& radii
)
    : positions(positions),
      pending_pos_update(false),
      colors(colors),
      pending_colors_update(false),
      radii(radii),
      pending_radii_update(false) {
    if (!((positions.size() == colors.size()) && (colors.size() == radii.size())
        )) {
        throw std::invalid_argument(
            std::format(
                "number of positions, colors, and radii must be the same, got "
                "{} positions, {} colors, {} radii",
                positions.size(),
                colors.size(),
                radii.size()
            )
        );
    }
}
// : mesh(PointCloud::make_mesh(positions, colors, radii)) {}

std::tuple<size_t, uint, uint> PointCloud::initialize_sphere_mesh() {
    // mesh first
    std::vector<glm::vec3> mesh_vertices;
    std::vector<uint32_t> mesh_indices;
    std::vector<glm::vec3> vertex_normals;

    generate_sphere(mesh_vertices, mesh_indices, vertex_normals, 1.0f, 10, 10);

    // vertex buffer
    uint mesh_vbo_id;
    gl::glGenBuffers(1, &mesh_vbo_id);
    gl::glBindBuffer(gl::GL_ARRAY_BUFFER, mesh_vbo_id);

    size_t vert_size = mesh_vertices.size() * sizeof(glm::vec3);
    size_t normals_size = vertex_normals.size() * sizeof(glm::vec3);

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
        mesh_vertices.data()
    );
    gl::glBufferSubData(
        gl::GL_ARRAY_BUFFER,
        vert_size,
        normals_size,
        vertex_normals.data()
    );

    // element buffer
    uint mesh_eab_id;
    gl::glGenBuffers(1, &mesh_eab_id);
    gl::glBindBuffer(gl::GL_ELEMENT_ARRAY_BUFFER, mesh_eab_id);

    gl::glBufferData(
        gl::GL_ELEMENT_ARRAY_BUFFER,
        mesh_indices.size() * sizeof(uint32_t),
        mesh_indices.data(),
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

    return std::make_tuple(mesh_indices.size(), mesh_vbo_id, mesh_eab_id);
}

uint PointCloud::initialize_pos_buffer() {
    uint pos_vbo_id;
    gl::glGenBuffers(1, &pos_vbo_id);

    gl::glBindBuffer(gl::GL_ARRAY_BUFFER, pos_vbo_id);

    gl::glBufferData(
        gl::GL_ARRAY_BUFFER,
        this->positions.size() * sizeof(glm::vec3),
        this->positions.data(),
        gl::GL_DYNAMIC_DRAW
    );

    gl::glVertexAttribPointer(
        2,
        3,
        gl::GL_FLOAT,
        gl::GL_FALSE,
        sizeof(glm::vec3),
        (void*)0
    );
    gl::glEnableVertexAttribArray(2);
    gl::glVertexAttribDivisor(2, 1);

    return pos_vbo_id;
}

uint PointCloud::initialize_radii_buffer() {
    uint vbo_id;
    gl::glGenBuffers(1, &vbo_id);

    gl::glBindBuffer(gl::GL_ARRAY_BUFFER, vbo_id);

    gl::glBufferData(
        gl::GL_ARRAY_BUFFER,
        this->radii.size() * sizeof(float),
        this->radii.data(),
        gl::GL_DYNAMIC_DRAW
    );

    gl::glVertexAttribPointer(
        3,
        1,
        gl::GL_FLOAT,
        gl::GL_FALSE,
        sizeof(float),
        (void*)0
    );
    gl::glEnableVertexAttribArray(3);
    gl::glVertexAttribDivisor(3, 1);

    return vbo_id;
}

uint PointCloud::initialize_color_buffer() {
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
        4,
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

void PointCloud::maybe_initialize() {
    if (this->render_thread_id.has_value()) {
        assert_thread(this->render_thread_id.value());
        return;
    }

    this->render_thread_id = std::this_thread::get_id();

    uint vao_id;
    gl::glGenVertexArrays(1, &vao_id);
    gl::glBindVertexArray(vao_id);

    auto [num_vertices, mesh_vbo_id, mesh_eab_id] =
        this->initialize_sphere_mesh();
    uint pos_vbo_id = this->initialize_pos_buffer();
    uint radii_vbo_id = this->initialize_radii_buffer();
    uint colors_vbo_id = this->initialize_color_buffer();

    gl::glBindVertexArray(0);

    this->gl_data.emplace(GLData(
        ShaderProgram(
            shader_source::point_cloud::vert,
            shader_source::point_cloud::frag
        ),
        vao_id,
        mesh_vbo_id,
        mesh_eab_id,
        pos_vbo_id,
        radii_vbo_id,
        colors_vbo_id,
        num_vertices
    ));
}

void PointCloud::update_positions(
    const std::vector<glm::vec3>& positions
) {
    this->positions = positions;
    this->pending_pos_update = true;
}

void PointCloud::update_colors(
    const std::vector<glm::vec3>& colors
) {
    this->colors = colors;
    this->pending_colors_update = true;
}

void PointCloud::update_radii(
    const std::vector<float>& radii
) {
    this->radii = radii;
    this->pending_radii_update = true;
}

void PointCloud::handle_updates() {
    if (this->pending_pos_update) {
        gl::glBindBuffer(gl::GL_ARRAY_BUFFER, this->gl_data.value().pos_vbo_id);
        gl::glBufferData(
            gl::GL_ARRAY_BUFFER,
            this->positions.size() * sizeof(glm::vec3),
            this->positions.data(),
            gl::GL_DYNAMIC_DRAW
        );
        this->pending_pos_update = false;
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

    if (this->pending_radii_update) {
        gl::glBindBuffer(
            gl::GL_ARRAY_BUFFER,
            this->gl_data.value().radii_vbo_id
        );
        gl::glBufferData(
            gl::GL_ARRAY_BUFFER,
            this->radii.size() * sizeof(float),
            this->radii.data(),
            gl::GL_DYNAMIC_DRAW
        );
        this->pending_radii_update = false;
    }
}

void PointCloud::render(
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
        gl_data.ball_vertex_count,
        gl::GL_UNSIGNED_INT,
        0,
        this->positions.size()
    );

    gl::glBindVertexArray(0);
}

}  // namespace _geom
}  // namespace slamd
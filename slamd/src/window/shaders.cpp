#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <spdlog/spdlog.h>
#include <format>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <slamd/io.hpp>
#include <slamd/shaders.hpp>
#include <stdexcept>

namespace slamd {

const std::size_t info_buffer_size = 512;

void ensure_shader_compiled(
    uint shader_id
) {
    int success;
    char info_log_buf[info_buffer_size];

    gl::glGetShaderiv(shader_id, gl::GL_COMPILE_STATUS, &success);

    if (!success) {
        gl::glGetShaderInfoLog(
            shader_id,
            info_buffer_size,
            nullptr,
            info_log_buf
        );
        throw std::runtime_error(
            std::format(
                "Couldn't compile shader: {}",
                std::string(info_log_buf)
            )
        );
    }
}

void ensure_shader_program_linked(
    uint program_id
) {
    int success;
    char info_log_buf[info_buffer_size];

    gl::glGetProgramiv(program_id, gl::GL_LINK_STATUS, &success);

    if (!success) {
        gl::glGetProgramInfoLog(
            program_id,
            info_buffer_size,
            nullptr,
            info_log_buf
        );
        throw std::runtime_error(
            std::format("Couldn't link program: {}", std::string(info_log_buf))
        );
    }
}

uint compile_vertex_shader(
    std::string source
) {
    // now we create the vertex shader
    const uint shader_id = gl::glCreateShader(gl::GL_VERTEX_SHADER);

    // read the shader source
    const char* shader_source_cstr = source.c_str();

    // set the shader's source
    gl::glShaderSource(shader_id, 1, &shader_source_cstr, nullptr);

    gl::glCompileShader(shader_id);

    ensure_shader_compiled(shader_id);

    return shader_id;
}

uint compile_fragment_shader(
    std::string source
) {
    const uint shader_id = gl::glCreateShader(gl::GL_FRAGMENT_SHADER);

    const char* shader_source_cstr = source.c_str();

    gl::glShaderSource(shader_id, 1, &shader_source_cstr, nullptr);
    gl::glCompileShader(shader_id);

    ensure_shader_compiled(shader_id);
    SPDLOG_INFO("Fragment shader compiled: id={}", shader_id);

    return shader_id;
}

uint make_shader_program(
    uint vertex_shader_id,
    uint fragment_shader_id
) {
    uint shader_program_id = gl::glCreateProgram();
    gl::glAttachShader(shader_program_id, vertex_shader_id);
    gl::glAttachShader(shader_program_id, fragment_shader_id);
    gl::glLinkProgram(shader_program_id);

    ensure_shader_program_linked(shader_program_id);

    return shader_program_id;
}

uint make_shader_program(
    std::string vertex_shader_source,
    std::string fragment_shader_source
) {
    auto vertex_shader_id = compile_vertex_shader(vertex_shader_source);
    auto fragment_shader_id = compile_fragment_shader(fragment_shader_source);

    auto shader_program_id =
        make_shader_program(vertex_shader_id, fragment_shader_id);

    gl::glDeleteShader(vertex_shader_id);
    gl::glDeleteShader(fragment_shader_id);

    return shader_program_id;
}

ShaderProgram::ShaderProgram(
    std::string vertex_shader_source,
    std::string fragment_shader_source
) {
    this->id =
        make_shader_program(vertex_shader_source, fragment_shader_source);
}

ShaderProgram::ShaderProgram(
    uint vertex_shader_id,
    uint fragment_shader_id
) {
    this->id = make_shader_program(vertex_shader_id, fragment_shader_id);
}

void ShaderProgram::use() {
    gl::glUseProgram(this->id);
}

template <>
void ShaderProgram::set_uniform<bool>(
    const std::string& name,
    const bool value
) const {
    gl::glUniform1i(this->get_uniform_location(name), static_cast<int>(value));
}

template <>
void ShaderProgram::set_uniform<int>(
    const std::string& name,
    const int value
) const {
    gl::glUniform1i(this->get_uniform_location(name), value);
}

template <>
void ShaderProgram::set_uniform<float>(
    const std::string& name,
    const float value
) const {
    gl::glUniform1f(this->get_uniform_location(name), value);
}

template <>
void ShaderProgram::set_uniform<glm::vec2>(
    const std::string& name,
    const glm::vec2 value
) const {
    gl::glUniform2fv(
        this->get_uniform_location(name),
        1,
        glm::value_ptr(value)
    );
}

template <>
void ShaderProgram::set_uniform<glm::vec3>(
    const std::string& name,
    const glm::vec3 value
) const {
    gl::glUniform3fv(
        this->get_uniform_location(name),
        1,
        glm::value_ptr(value)
    );
}

template <>
void ShaderProgram::set_uniform<glm::vec4>(
    const std::string& name,
    const glm::vec4 value
) const {
    gl::glUniform4fv(
        this->get_uniform_location(name),
        1,
        glm::value_ptr(value)
    );
}

// Matrices? Why not!
template <>
void ShaderProgram::set_uniform<glm::mat4>(
    const std::string& name,
    const glm::mat4 value
) const {
    gl::glUniformMatrix4fv(
        this->get_uniform_location(name),
        1,
        gl::GL_FALSE,
        glm::value_ptr(value)
    );
}

int ShaderProgram::get_uniform_location(
    const std::string& name
) const {
    return gl::glGetUniformLocation(id, name.c_str());
}

}  // namespace slamd

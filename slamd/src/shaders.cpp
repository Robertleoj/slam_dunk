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
    gl::GLuint shader_id
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
    gl::GLuint program_id
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

gl::GLuint compile_vertex_shader(
    std::string source
) {
    // now we create the vertex shader
    const gl::GLuint shader_id = gl::glCreateShader(gl::GL_VERTEX_SHADER);

    // read the shader source
    const char* shader_source_cstr = source.c_str();

    // set the shader's source
    gl::glShaderSource(shader_id, 1, &shader_source_cstr, nullptr);

    gl::glCompileShader(shader_id);

    ensure_shader_compiled(shader_id);

    return shader_id;
}

gl::GLuint compile_vertex_shader(
    fs::path path
) {
    const std::string shader_source = read_file_text(path);
    return compile_vertex_shader(shader_source);
}

gl::GLuint compile_fragment_shader(
    std::string source
) {
    const gl::GLuint shader_id = gl::glCreateShader(gl::GL_FRAGMENT_SHADER);

    const char* shader_source_cstr = source.c_str();

    gl::glShaderSource(shader_id, 1, &shader_source_cstr, nullptr);
    gl::glCompileShader(shader_id);

    ensure_shader_compiled(shader_id);
    spdlog::info("Fragment shader compiled: id={}", shader_id);

    return shader_id;
}

gl::GLuint compile_fragment_shader(
    fs::path path
) {
    const std::string shader_source = read_file_text(path);
    return compile_fragment_shader(shader_source);
}

gl::GLuint make_shader_program(
    gl::GLuint vertex_shader_id,
    gl::GLuint fragment_shader_id
) {
    gl::GLuint shader_program_id = gl::glCreateProgram();
    gl::glAttachShader(shader_program_id, vertex_shader_id);
    gl::glAttachShader(shader_program_id, fragment_shader_id);
    gl::glLinkProgram(shader_program_id);

    ensure_shader_program_linked(shader_program_id);

    return shader_program_id;
}

gl::GLuint make_shader_program(
    fs::path vertex_shader_path,
    fs::path fragment_shader_path
) {
    auto vertex_shader_id = compile_vertex_shader(vertex_shader_path);
    auto fragment_shader_id = compile_fragment_shader(fragment_shader_path);

    auto shader_program_id =
        make_shader_program(vertex_shader_id, fragment_shader_id);

    gl::glDeleteShader(vertex_shader_id);
    gl::glDeleteShader(fragment_shader_id);

    return shader_program_id;
}

ShaderProgram::ShaderProgram(
    fs::path vertex_shader_path,
    fs::path fragment_shader_path
) {
    this->id = make_shader_program(vertex_shader_path, fragment_shader_path);
}

ShaderProgram::ShaderProgram(
    gl::GLuint vertex_shader_id,
    gl::GLuint fragment_shader_id
) {
    this->id = make_shader_program(vertex_shader_id, fragment_shader_id);
}

void ShaderProgram::use() {
    gl::glUseProgram(this->id);
}

template <>
void ShaderProgram::setUniform<bool>(
    const std::string& name,
    const bool value
) const {
    gl::glUniform1i(getUniformLocation(name), static_cast<int>(value));
}

template <>
void ShaderProgram::setUniform<int>(
    const std::string& name,
    const int value
) const {
    int variable_location = gl::glGetUniformLocation(this->id, name.c_str());

    gl::glUniform1i(variable_location, value);
}

template <>
void ShaderProgram::setUniform<float>(
    const std::string& name,
    const float value
) const {
    int variable_location = gl::glGetUniformLocation(this->id, name.c_str());

    gl::glUniform1f(variable_location, value);
}

template <>
void ShaderProgram::setUniform<glm::vec2>(
    const std::string& name,
    const glm::vec2 value
) const {
    gl::glUniform2fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

template <>
void ShaderProgram::setUniform<glm::vec3>(
    const std::string& name,
    const glm::vec3 value
) const {
    gl::glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

template <>
void ShaderProgram::setUniform<glm::vec4>(
    const std::string& name,
    const glm::vec4 value
) const {
    gl::glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

// Matrices? Why not!
template <>
void ShaderProgram::setUniform<glm::mat4>(
    const std::string& name,
    const glm::mat4 value
) const {
    gl::glUniformMatrix4fv(
        getUniformLocation(name),
        1,
        gl::GL_FALSE,
        glm::value_ptr(value)
    );
}

}  // namespace slamd

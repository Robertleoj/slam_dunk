#pragma once
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <filesystem>

namespace fs = std::filesystem;
namespace slamd {

void ensure_shader_compiled(gl::GLuint shader_id);

void ensure_shader_program_linked(gl::GLuint program_id);

gl::GLuint compile_vertex_shader(std::string source);
gl::GLuint compile_vertex_shader(fs::path path);

gl::GLuint
make_shader_program(fs::path vertex_shader_path, fs::path fragment_shader_path);

gl::GLuint compile_fragment_shader(std::string source);
gl::GLuint compile_fragment_shader(fs::path path);

gl::GLuint
make_shader_program(gl::GLuint vertex_shader_id, gl::GLuint fragment_shader_id);

class ShaderProgram {
   public:
    gl::GLuint id;
    void use();

    ShaderProgram(fs::path vertex_shader_path, fs::path fragment_shader_path);
    ShaderProgram(gl::GLuint vertex_shader_id, gl::GLuint fragment_shader_id);

    template <typename T>
    void set_uniform(const std::string& name, const T value) const;

   private:
    gl::GLint getUniformLocation(
        const std::string& name
    ) const {
        return gl::glGetUniformLocation(id, name.c_str());
    }
};

}  // namespace slamd
#pragma once
#include <string>

namespace slamd {

void ensure_shader_compiled(uint shader_id);

void ensure_shader_program_linked(uint program_id);

uint compile_vertex_shader(std::string source);

uint make_shader_program(
    std::string vertex_shader_source,
    std::string fragment_shader_source
);

uint compile_fragment_shader(std::string source);

uint make_shader_program(uint vertex_shader_id, uint fragment_shader_id);

class ShaderProgram {
   public:
    uint id;
    void use();

    ShaderProgram(
        std::string vertex_shader_source,
        std::string fragment_shader_source
    );
    ShaderProgram(uint vertex_shader_id, uint fragment_shader_id);

    template <typename T>
    void set_uniform(const std::string& name, const T value) const;

   private:
    int get_uniform_location(const std::string& name) const;
};

}  // namespace slamd
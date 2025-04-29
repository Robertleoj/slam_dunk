#pragma once
#include <string>

namespace slamdw {

void ensure_shader_compiled(uint32_t shader_id);

void ensure_shader_program_linked(uint32_t program_id);

uint32_t compile_vertex_shader(std::string source);

uint32_t make_shader_program(
    std::string vertex_shader_source,
    std::string fragment_shader_source
);

uint32_t compile_fragment_shader(std::string source);

uint32_t make_shader_program(uint32_t vertex_shader_id, uint32_t fragment_shader_id);

class ShaderProgram {
   public:
    uint32_t id;
    void use();

    ShaderProgram(
        std::string vertex_shader_source,
        std::string fragment_shader_source
    );
    ShaderProgram(uint32_t vertex_shader_id, uint32_t fragment_shader_id);

    template <typename T>
    void set_uniform(const std::string& name, const T value) const;

   private:
    int get_uniform_location(const std::string& name) const;
};

}  // namespace slamd
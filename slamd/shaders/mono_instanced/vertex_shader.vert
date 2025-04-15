#version 330 core

layout(location = 0) in vec3 a_model_vertex_pos;
layout(location = 1) in vec3 a_normal;

layout(location = 2) in vec4 a_transform_1;
layout(location = 3) in vec4 a_transform_2;
layout(location = 4) in vec4 a_transform_3;
layout(location = 5) in vec4 a_transform_4;

layout(location = 6) in vec3 a_color;

out vec3 o_vertex_color;
out vec3 o_normal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main() {
    mat4 a_transform =
        mat4(a_transform_1, a_transform_2, a_transform_3, a_transform_4);

    o_normal = mat3(transpose(inverse(u_model))) * a_normal;

    gl_Position = u_projection * u_view * u_model * a_transform *
                  vec4(a_model_vertex_pos, 1.0);
    o_vertex_color = a_color;
}

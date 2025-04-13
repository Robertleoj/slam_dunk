#version 330 core

in vec3 o_normal;
in vec3 o_vertex_color;

out vec4 FragColor;

uniform vec3 u_light_dir;
uniform float u_min_brightness;

void main() {
    vec3 norm = normalize(o_normal);
    float diff =
        max(dot(norm, normalize(u_light_dir)),
            u_min_brightness);  // never too dark
    FragColor = vec4(o_vertex_color * diff, 1.0);
}
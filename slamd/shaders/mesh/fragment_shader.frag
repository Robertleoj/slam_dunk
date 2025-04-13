#version 330 core

in vec3 Normal;
in vec3 vertex_color;

out vec4 FragColor;

uniform vec3 light_dir;
uniform float min_brightness;

void main() {
    vec3 norm = normalize(Normal);
    float diff =
        max(dot(norm, normalize(light_dir)), min_brightness);  // never too dark
    FragColor = vec4(vertex_color * diff, 1.0);
}
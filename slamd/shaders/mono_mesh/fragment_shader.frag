#version 330 core

in vec3 Normal;
out vec4 FragColor;

uniform vec3 color;
uniform vec3 light_dir;

void main() {
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, normalize(light_dir)), 0.3);  // never too dark
    FragColor = vec4(color * diff, 1.0);
}

#version 330 core

out vec4 FragColor;

uniform vec3 uColor;
uniform float uAlpha;

void main() {
    FragColor = vec4(uColor, uAlpha);
}

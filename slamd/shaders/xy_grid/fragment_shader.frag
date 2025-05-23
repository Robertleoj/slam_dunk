#version 330 core

in vec3 viewPos;

out vec4 FragColor;

uniform vec3 uColor;
uniform float uExtraAlpha;
uniform float uScale;

void main() {
    float dist = length(viewPos);

    float alpha = clamp(
        1.0 - (dist / (uScale * 100.0)), 0.0, 1.0
    );  // adjust 20.0 to control fade range

    FragColor = vec4(uColor, alpha * uExtraAlpha);
}

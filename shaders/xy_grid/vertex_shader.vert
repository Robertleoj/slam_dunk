#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

out vec3 viewPos; // Position in view space

void main() {
    vec4 worldPos = uModel * vec4(aPos, 1.0);
    vec4 vPos = uView * worldPos;
    viewPos = vPos.xyz;

    gl_Position = uProjection * vPos;
}

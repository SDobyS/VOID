#version 460 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 vColor;
out vec2 vTexCoord;

uniform mat4 u_ViewProj;
uniform mat4 u_Model;

void main() {
    gl_Position = u_ViewProj * u_Model * vec4(aPos, 0.0, 1.0);
    vColor = aColor;
    vTexCoord = aTexCoord;
}
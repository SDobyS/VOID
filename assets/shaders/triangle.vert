#version 460 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 vertexColor;
out vec2 TexCoord;

uniform float aspect;

void main() {
    gl_Position = vec4(aPos.x * aspect, aPos.y, 0.0, 1.0);
    vertexColor = aColor;
    TexCoord = aTexCoord;
}

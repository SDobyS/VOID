#version 460 core

in vec3 vColor;
in vec2 vTexCoord;
out vec4 FragColor;

uniform sampler2D ourTexture;

void main() {
    FragColor = texture(ourTexture, vTexCoord) * vec4(vColor, 1.0);
}
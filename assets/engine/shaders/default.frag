#version 460 core

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;

out vec4 FragColor;

uniform sampler2D u_Textures[32];

void main() {
    int index = int(v_TexIndex + 0.5);
    vec4 texColor = texture(u_Textures[index], v_TexCoord);
    FragColor = texColor * v_Color;
}
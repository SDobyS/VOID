#version 460 core

const vec2 position[3] = vec2[3](
        vec2( 0.0,  0.5),
        vec2(-0.5, -0.5),
        vec2( 0.5, -0.5)
);

const vec3 colors[3] = vec3[3](
        vec3(1.0, 0.0, 0.0),
        vec3(0.0, 1.0, 0.0),
        vec3(0.0, 0.0, 1.0)
);

out vec3 vertexColor;

void main() {
    gl_Position = vec4(position[gl_VertexID], 0.0, 1.0);
    vertexColor = colors[gl_VertexID];
}
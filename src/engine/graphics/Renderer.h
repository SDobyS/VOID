#pragma once
#include <glad/gl.h>
#include "Shader.h"
#include "Texture.h"
#include "Camera/OrthographicCamera.h"
#include <glm/glm.hpp>

class Renderer {
public:
    static void Init();
    static void BeginScene(const OrthographicCamera& camera);
    static void DrawQuad(const Texture& texture, const glm::vec2& position, const glm::vec2& size, const glm::vec4& color = {1,1,1,1});

private:
    static Shader* s_DefaultShader;
    static GLuint s_QuadVAO;
};
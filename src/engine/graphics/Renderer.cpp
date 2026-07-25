#include "Renderer.h"
#include "utils/Log.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Shader* Renderer::s_DefaultShader = nullptr;
GLuint Renderer::s_QuadVAO = 0;

void Renderer::Init() {
    s_DefaultShader = new Shader();
    s_DefaultShader->Load("../assets/engine/shaders/default.vert", "../assets/engine/shaders/default.frag");

    float vertices[] = {
        // pos         // color             // tex
        0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    0.0f, 0.0f,
        1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f,
        1.0f, 1.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f,
        0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f
    };
    unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

    glGenVertexArrays(1, &s_QuadVAO);
    GLuint vbo, ebo;
    glGenBuffers(1, &vbo); glGenBuffers(1, &ebo);

    glBindVertexArray(s_QuadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, vbo); glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), nullptr); glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float))); glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), reinterpret_cast<void*>(5 * sizeof(float))); glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo); glDeleteBuffers(1, &ebo);
    Log::Success("Renderer initialized");
}

void Renderer::BeginScene(const OrthographicCamera& camera) {
    s_DefaultShader->Use();
    s_DefaultShader->SetUniformMat4f("u_ViewProj", glm::value_ptr(camera.GetViewProjectionMatrix()));
    glm::mat4 model = glm::mat4(1.0f);
    s_DefaultShader->SetUniformMat4f("u_Model", glm::value_ptr(model));
}

void Renderer::DrawQuad(const Texture& texture, const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color) {
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));

    if (rotation != 0.0f) {
        model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
    }

    model = glm::scale(model, glm::vec3(size, 1.0f));
    s_DefaultShader->Use();
    s_DefaultShader->SetUniformMat4f("u_Model", glm::value_ptr(model));
    s_DefaultShader->SetUniform4f("u_Color", color);
    texture.Bind();
    s_DefaultShader->SetUniform1i("u_Texture", 0);
    glBindVertexArray(s_QuadVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
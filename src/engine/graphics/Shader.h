#pragma once

#include <glad/gl.h>

#include <string>
#include <iostream>
#include <glm/glm.hpp>

class Shader {
public:
    Shader() = default;
    ~Shader();

    void SetUniformMat4f(const std::string& name, const float* data);
    void SetUniform4f(const std::string& name, const glm::vec4& value);
    void SetUniform1i(const std::string& name, int value);

    bool Load(const std::string& vertexPath, const std::string& fragmentPath);
    void Use() const;
    GLuint GetID() const;

private:
    GLuint m_ID = 0;
    std::string ReadFile(const std::string& path);
};
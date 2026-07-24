#include "Shader.h"
#include "../utils/Log.h"
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    // Read
    std::string vertexSource = ReadFile(vertexPath);
    std::string fragmentSource = ReadFile(fragmentPath);

    // Comp
    unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
    unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

    // Link
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success) {
        char infoLog[1024];
        glGetProgramInfoLog(shaderProgram, 1024, nullptr, infoLog);
        Log::Error("Shader program linking failed", std::string(infoLog));
        glDeleteProgram(shaderProgram);
        shaderProgram = 0;
    }
    Log::Success("Shader program linked successfully");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() { if (shaderProgram) glDeleteProgram(shaderProgram); }
void Shader::Bind() const { glUseProgram(shaderProgram); }
void Shader::Unbind() { glUseProgram(0); }
void Shader::SetUniformMat4f(const std::string& name, const float* matrixData) const {
    int location = glGetUniformLocation(shaderProgram, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, matrixData);
}

std::string Shader::ReadFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        Log::Error("Failed to open shader file", path);
        return {};
    }
    Log::Success("Shader file opened successfully");
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    unsigned int shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        Log::Error("Shader compilation failed", std::string(infoLog));
        glDeleteShader(shader);
        return 0;
    }
    Log::Success("Shader compiled successfully");
    return shader;
}

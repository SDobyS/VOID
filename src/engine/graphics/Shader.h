#pragma once
#include <glad/gl.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <unordered_map>

namespace voidx {
    class Shader {
    public:
        Shader() = default;
        ~Shader();

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        Shader(Shader&& other) noexcept;
        Shader& operator=(Shader&& other) noexcept;

        void SetUniformMat4f(const std::string& name, const float* data);
        void SetUniform4f(const std::string& name, const glm::vec4& value);
        void SetUniform1i(const std::string& name, int value);
        void SetUniform1iv(const std::string& name, int count, const int* data);

        bool Load(const std::string& vertexPath, const std::string& fragmentPath);
        void Use() const;
        GLuint GetID() const { return m_ID; }

    private:
        GLuint m_ID = 0;
        std::string ReadFile(const std::string& path);
        std::unordered_map<std::string, GLint> m_UniformLocationCache;
        GLint GetUniformLocation(const std::string& name);
    };
}
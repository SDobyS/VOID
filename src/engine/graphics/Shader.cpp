#include "Shader.h"
#include "../utils/Log.h"

namespace voidx {
    std::string Shader::ReadFile(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            Log::Error("Failed to open shader file", path);
            return {};
        }
        std::stringstream ss;
        ss << file.rdbuf();
        return ss.str();
    }

    bool Shader::Load(const std::string& vertexPath, const std::string& fragmentPath) {
        std::string vertexSource = ReadFile(vertexPath);
        std::string fragmentSource = ReadFile(fragmentPath);

        if (vertexSource.empty() || fragmentSource.empty()) {
            Log::Error("Failed to load shader files", "Vertex or fragment source is empty");
            return false;
        }

        const char* vertexCode = vertexSource.c_str();
        const char* fragmentCode = fragmentSource.c_str();

        GLint success;
        char infoLog[1024];

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexCode, nullptr);
        glCompileShader(vertexShader);
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertexShader, 1024, nullptr, infoLog);
            Log::Error("Vertex shader compilation failed", std::string(infoLog));
            glDeleteShader(vertexShader);
            return false;
        }

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentCode, nullptr);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentShader, 1024, nullptr, infoLog);
            Log::Error("Fragment shader compilation failed", std::string(infoLog));
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            return false;
        }

        m_ID = glCreateProgram();
        glAttachShader(m_ID, vertexShader);
        glAttachShader(m_ID, fragmentShader);
        glLinkProgram(m_ID);
        glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(m_ID, 1024, nullptr, infoLog);
            Log::Error("Shader program linking failed", std::string(infoLog));
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            glDeleteProgram(m_ID);
            m_ID = 0;
            return false;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return true;
    }

    GLint Shader::GetUniformLocation(const std::string& name) {
        auto it = m_UniformLocationCache.find(name);
        if (it != m_UniformLocationCache.end()) return it->second;

        GLint loc = glGetUniformLocation(m_ID, name.c_str());
        m_UniformLocationCache[name] = loc;
        return loc;
    }

    void Shader::SetUniformMat4f(const std::string& name, const float* data) {
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, data);
    }

    void Shader::SetUniform4f(const std::string& name, const glm::vec4& value) {
        glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
    }

    void Shader::SetUniform1i(const std::string& name, int value) {
        glUniform1i(GetUniformLocation(name), value);
    }

    void Shader::SetUniform1iv(const std::string& name, int count, const int* data) {
        glUniform1iv(GetUniformLocation(name), count, data);
    }

    void Shader::Use() const { glUseProgram(m_ID); }
    Shader::~Shader() { if (m_ID) glDeleteProgram(m_ID); }
}
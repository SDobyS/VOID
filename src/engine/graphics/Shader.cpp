#include "Shader.h"
#include <utility>
#include "../utils/Log.h"

namespace voidx {
    Shader::Shader(Shader&& other) noexcept
    : m_ID(std::exchange(other.m_ID, 0)),
      m_UniformLocationCache(std::move(other.m_UniformLocationCache)) {
        Log::Debug("Shader", "Move constructor called");
    }

    Shader& Shader::operator=(Shader&& other) noexcept {
        if (this != &other) {
            Log::Debug("Shader", "Move assignment called. Deleting old program ID: " + std::to_string(m_ID));

            if (m_ID) glDeleteProgram(m_ID);

            m_ID = std::exchange(other.m_ID, 0);
            m_UniformLocationCache = std::move(other.m_UniformLocationCache);
        }
        return *this;
    }

    std::string Shader::ReadFile(const std::string& path) {
        Log::Info("Shader", "Reading file: " + path);
        std::ifstream file(path);
        if (!file.is_open()) {
            Log::Error("Shader", "Failed to open shader file: " + path);
            return {};
        }
        std::stringstream ss;
        ss << file.rdbuf();
        return ss.str();
    }

    bool Shader::Load(const std::string& vertexPath, const std::string& fragmentPath) {
        Log::Info("Shader", "Loading shader program...");

        std::string vertexSource = ReadFile(vertexPath);
        std::string fragmentSource = ReadFile(fragmentPath);

        if (vertexSource.empty() || fragmentSource.empty()) {
            Log::Error("Shader", "Vertex or fragment source is empty.");
            return false;
        }

        const char* vertexCode = vertexSource.c_str();
        const char* fragmentCode = fragmentSource.c_str();

        GLint success;

        Log::Info("Shader", "Compiling Vertex Shader...");
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexCode, nullptr);
        glCompileShader(vertexShader);
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLint logLength = 0;
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);
            std::vector<char> infoLog(logLength > 1 ? logLength : 1);
            glGetShaderInfoLog(vertexShader, logLength, nullptr, infoLog.data());
            Log::Error("Shader", "Vertex shader compilation failed: " + std::string(infoLog.data()));
            glDeleteShader(vertexShader);
            return false;
        }
        Log::Success("Shader", "Vertex shader compiled.");

        Log::Info("Shader", "Compiling Fragment Shader...");
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentCode, nullptr);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLint logLength = 0;
            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);
            std::vector<char> infoLog(logLength > 1 ? logLength : 1);
            glGetShaderInfoLog(fragmentShader, logLength, nullptr, infoLog.data());
            Log::Error("Shader", "Fragment shader compilation failed: " + std::string(infoLog.data()));
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            return false;
        }
        Log::Success("Shader", "Fragment shader compiled.");

        Log::Info("Shader", "Linking Shader Program...");
        m_ID = glCreateProgram();
        glAttachShader(m_ID, vertexShader);
        glAttachShader(m_ID, fragmentShader);
        glLinkProgram(m_ID);
        glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
        if (!success) {
            GLint logLength = 0;
            glGetProgramiv(m_ID, GL_INFO_LOG_LENGTH, &logLength);
            std::vector<char> infoLog(logLength > 1 ? logLength : 1);
            glGetProgramInfoLog(m_ID, logLength, nullptr, infoLog.data());
            Log::Error("Shader", "Shader program linking failed: " + std::string(infoLog.data()));
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            glDeleteProgram(m_ID);
            m_ID = 0;
            return false;
        }

        Log::Success("Shader", "Shader program linked successfully (ID: " + std::to_string(m_ID) + ")");
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return true;
    }

    GLint Shader::GetUniformLocation(const std::string& name) {
        auto it = m_UniformLocationCache.find(name);
        if (it != m_UniformLocationCache.end()) {
            return it->second;
        }

        Log::Debug("Shader", "Uniform cache miss. Querying GL: " + name);
        GLint loc = glGetUniformLocation(m_ID, name.c_str());
        if (loc == -1) {
            Log::Warning("Shader", "Uniform '" + name + "' not found or optimized out.");
        }
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

    void Shader::Use() const {
        glUseProgram(m_ID);
    }

    Shader::~Shader() {
        if (m_ID) {
            Log::Debug("Shader", "Destroyed program ID: " + std::to_string(m_ID));
            glDeleteProgram(m_ID);
        }
    }
}
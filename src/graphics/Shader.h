#pragma once
#include <glad/gl.h>
#include <string>

class Shader {
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();
    void Bind() const;
    static void Unbind();
    void SetUniformMat4f(const std::string& name, const float* matrixData) const;
    unsigned int GetID() const { return shaderProgram; }

private:
    static std::string ReadFile(const std::string& path);
    static unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int shaderProgram;
};
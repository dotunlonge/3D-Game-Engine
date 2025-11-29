#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

class Shader {
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    void SetUniform1i(const std::string& name, int value);
    void SetUniform1f(const std::string& name, float value);
    void SetUniform3f(const std::string& name, const glm::vec3& value);
    void SetUniform4f(const std::string& name, const glm::vec4& value);
    void SetUniformMat4(const std::string& name, const glm::mat4& matrix);

    unsigned int GetID() const { return m_RendererID; }

private:
    std::string ReadFile(const std::string& filepath);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int CreateShader(const std::string& vertexSource, const std::string& fragmentSource);
    int GetUniformLocation(const std::string& name);

    unsigned int m_RendererID;
    std::unordered_map<std::string, int> m_UniformLocationCache;
};


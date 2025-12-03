#include "Shader.h"
#include "../core/Logger.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <glad/glad.h>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexSource = ReadFile(vertexPath);
    std::string fragmentSource = ReadFile(fragmentPath);
    m_RendererID = CreateShader(vertexSource, fragmentSource);
}

Shader::~Shader() {
    glDeleteProgram(m_RendererID);
}

void Shader::Bind() const {
    glUseProgram(m_RendererID);
}

void Shader::Unbind() const {
    glUseProgram(0);
}

std::string Shader::ReadFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        LOG_ERROR("Failed to open shader file: " + filepath);
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> message(length);
        glGetShaderInfoLog(id, length, &length, message.data());
        
        std::string shaderType = (type == GL_VERTEX_SHADER ? "vertex" : 
                                 type == GL_FRAGMENT_SHADER ? "fragment" : 
                                 type == GL_GEOMETRY_SHADER ? "geometry" : "unknown");
        
        LOG_ERROR("Failed to compile " + shaderType + " shader:");
        LOG_ERROR(std::string(message.data()));
        
        // Print source lines for context
        std::istringstream stream(source);
        std::string line;
        int lineNum = 1;
        LOG_ERROR("Shader source:");
        while (std::getline(stream, line) && lineNum <= 20) { // Show first 20 lines
            LOG_ERROR("  " + std::to_string(lineNum) + ": " + line);
            lineNum++;
        }
        
        glDeleteShader(id);
        return 0;
    }
    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexSource, const std::string& fragmentSource) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexSource);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);
    
    if (vs == 0 || fs == 0) {
        LOG_ERROR("Failed to compile shaders, cannot create program");
        if (vs != 0) glDeleteShader(vs);
        if (fs != 0) glDeleteShader(fs);
        glDeleteProgram(program);
        return 0;
    }

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    
    // Check linking
    int linkSuccess;
    glGetProgramiv(program, GL_LINK_STATUS, &linkSuccess);
    if (!linkSuccess) {
        int length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> message(length);
        glGetProgramInfoLog(program, length, &length, message.data());
        LOG_ERROR("Failed to link shader program:");
        LOG_ERROR(std::string(message.data()));
        
        glDeleteShader(vs);
        glDeleteShader(fs);
        glDeleteProgram(program);
        return 0;
    }
    
    glValidateProgram(program);
    
    // Check validation
    int validateSuccess;
    glGetProgramiv(program, GL_VALIDATE_STATUS, &validateSuccess);
    if (!validateSuccess) {
        int length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> message(length);
        glGetProgramInfoLog(program, length, &length, message.data());
        LOG_WARN("Shader program validation warning:");
        LOG_WARN(std::string(message.data()));
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int Shader::GetUniformLocation(const std::string& name) {
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
        return m_UniformLocationCache[name];
    }
    int location = glGetUniformLocation(m_RendererID, name.c_str());
    m_UniformLocationCache[name] = location;
    return location;
}

void Shader::SetUniform1i(const std::string& name, int value) {
    glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniform1f(const std::string& name, float value) {
    glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniform3f(const std::string& name, const glm::vec3& value) {
    glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
}

void Shader::SetUniform4f(const std::string& name, const glm::vec4& value) {
    glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
}

void Shader::SetUniformMat4(const std::string& name, const glm::mat4& matrix) {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}


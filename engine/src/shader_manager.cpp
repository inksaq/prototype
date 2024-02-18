#include "shader_manager.h"
#include <iostream>
#include <fstream>
#include <sstream>

ShaderManager::ShaderManager() {}

ShaderManager::~ShaderManager() {
    for (auto& shader : shaders) {
        glDeleteProgram(shader.second);
    }
}


GLuint ShaderManager::compileShaderFromMemory(const std::span<const unsigned char>& shaderData, GLenum shaderType) const {
    GLuint shaderID = glCreateShader(shaderType);
    const GLchar* source = reinterpret_cast<const GLchar*>(shaderData.data());
    GLint length = static_cast<GLint>(shaderData.size());
    glShaderSource(shaderID, 1, &source, &length);
    glCompileShader(shaderID);

    // Check compilation status
    GLint success;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shaderID, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return shaderID;
}


GLuint ShaderManager::linkProgram(GLuint vertexShaderID, GLuint fragmentShaderID) const {
    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    GLint success;
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(programID, 512, nullptr, infoLog);
        std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    return programID;
}

GLuint ShaderManager::loadShaderFromMemory(const std::span<const unsigned char>& vertexShaderData, 
                                           const std::span<const unsigned char>& fragmentShaderData, 
                                           const std::string& shaderName) {
    GLuint vertexShaderID = compileShaderFromMemory(vertexShaderData, GL_VERTEX_SHADER);
    GLuint fragmentShaderID = compileShaderFromMemory(fragmentShaderData, GL_FRAGMENT_SHADER);

    GLuint shaderProgramID = linkProgram(vertexShaderID, fragmentShaderID);
    shaders[shaderName] = shaderProgramID;

    return shaderProgramID;
}

GLuint ShaderManager::getShader(const std::string& shaderName) const {
    auto it = shaders.find(shaderName);
    if (it != shaders.end()) {
        return it->second;
    } else {
        std::cerr << "Shader not found: " << shaderName << std::endl;
        return 0;
    }
}

void ShaderManager::deleteShader(const std::string& shaderName) {
    auto it = shaders.find(shaderName);
    if (it != shaders.end()) {
        glDeleteProgram(it->second);
        shaders.erase(it);
    }
}

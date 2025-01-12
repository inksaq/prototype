#include "includes/render/shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

namespace Core::Render {

Shader::Shader(const std::vector<unsigned char>& vertexShaderCode,
               const std::vector<unsigned char>& fragmentShaderCode) {
    std::string vertexCode(vertexShaderCode.begin(), vertexShaderCode.end());
    std::string fragmentCode(fragmentShaderCode.begin(), fragmentShaderCode.end());
    initialize(vertexCode.c_str(), fragmentCode.c_str());
}

    Shader::Shader(const std::span<const unsigned char>& vertexShaderData,
               const std::span<const unsigned char>& fragmentShaderData) {
    initializeFromBinary(vertexShaderData, fragmentShaderData);
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexCode = readFile(vertexPath);
    std::string fragmentCode = readFile(fragmentPath);
    initialize(vertexCode.c_str(), fragmentCode.c_str());
}

Shader::~Shader() {
    glDeleteProgram(ID);
}

void Shader::initialize(const char* vertexSource, const char* fragmentSource) {
    GLuint vertex = compileShader(vertexSource, GL_VERTEX_SHADER);
    GLuint fragment = compileShader(fragmentSource, GL_FRAGMENT_SHADER);

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

    void Shader::initializeFromBinary(const std::span<const unsigned char>& vertexData,
                                const std::span<const unsigned char>& fragmentData) {
    GLuint vertexShader = compileShaderFromMemory(vertexData, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShaderFromMemory(fragmentData, GL_FRAGMENT_SHADER);

    ID = linkProgram(vertexShader, fragmentShader);
}

    GLuint Shader::compileShaderFromMemory(const std::span<const unsigned char>& shaderData,
                                  GLenum shaderType) const {
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

    GLuint Shader::linkProgram(GLuint vertexShaderID, GLuint fragmentShaderID) const {
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

    void Shader::use() {
    glUseProgram(ID);

    // Verify uniform locations
    GLint texLoc = glGetUniformLocation(ID, "texture1");
    GLint useTexLoc = glGetUniformLocation(ID, "useTexture");
    std::cout << "Texture uniform location: " << texLoc << std::endl;
    std::cout << "UseTexture uniform location: " << useTexLoc << std::endl;
}

void Shader::dispose() {
    glUseProgram(0);
}

GLint Shader::getUniformLocation(const std::string& name) const {
    auto it = uniformLocationCache.find(name);
    if (it != uniformLocationCache.end()) {
        return it->second;
    }
    GLint location = glGetUniformLocation(ID, name.c_str());
    uniformLocationCache[name] = location;
    return location;
}

// Uniform setters
void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(getUniformLocation(name), (int)value);
}

void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(getUniformLocation(name), value);
}

void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(getUniformLocation(name), value);
}



void Shader::setVec2(const std::string& name, const glm::vec2& value) const {
    glUniform2fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const {
    glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::setMat2(const std::string& name, const glm::mat2& mat) const {
    glUniformMatrix2fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setMat3(const std::string& name, const glm::mat3& mat) const {
    glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
}

// Texture utilities
void Shader::setTexture(const std::string& name, GLuint textureUnit) const {
    glUniform1i(getUniformLocation(name), textureUnit);
}

void Shader::bindTexture(GLuint textureID, GLuint unit) const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

// Helper methods
GLuint Shader::compileShader(const char* source, GLenum type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    checkCompileErrors(shader, type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT");
    return shader;
}

void Shader::checkCompileErrors(GLuint shader, const std::string& type) {
    GLint success;
    char infoLog[1024];

    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type
                      << "\n" << infoLog << "\n -- --------------------------------------------------- -- "
                      << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type
                      << "\n" << infoLog << "\n -- --------------------------------------------------- -- "
                      << std::endl;
        }
    }
}

std::string Shader::readFile(const std::string& filePath) {
    std::string content;
    try {
        std::ifstream file(filePath);
        std::stringstream stream;
        stream << file.rdbuf();
        file.close();
        content = stream.str();
    }
    catch (std::ifstream::failure& e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << filePath << std::endl;
    }
    return content;
}

} // namespace Core::Render
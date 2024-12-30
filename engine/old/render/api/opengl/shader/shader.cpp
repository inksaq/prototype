//
// Created by Administrator on 5/17/2024.
//

#include "shader.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace backend::renderapi::opengl {
    Shader::Shader(const std::vector<unsigned char>& vertexShaderCode, const std::vector<unsigned char>& fragmentShaderCode) {
        std::string vertexCode(vertexShaderCode.begin(), vertexShaderCode.end());
        std::string fragmentCode(fragmentShaderCode.begin(), fragmentShaderCode.end());
        initialize(vertexCode.c_str(), fragmentCode.c_str());
    }

    Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
        std::string vertexCode = readFile(vertexPath);
        std::string fragmentCode = readFile(fragmentPath);
        initialize(vertexCode.c_str(), fragmentCode.c_str());
    }

    void Shader::initialize(const char* vertexSource, const char* fragmentSource) {
        unsigned int vertex = compileShader(vertexSource, GL_VERTEX_SHADER);
        unsigned int fragment = compileShader(fragmentSource, GL_FRAGMENT_SHADER);

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    std::string Shader::readFile(const std::string& filePath) {
        std::ifstream shaderFile;
        std::stringstream shaderStream;

        // Ensure ifstream objects can throw exceptions:
        shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            // Open files
            shaderFile.open(filePath);
            shaderStream << shaderFile.rdbuf(); // Read file's buffer contents into streams
            shaderFile.close();
            // Convert stream into string
            return shaderStream.str();
        } catch (std::ifstream::failure e) {
            std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << filePath << std::endl;
            return "";
        }
    }

    unsigned int Shader::compileShader(const char* shaderCode, GLenum type) {
        unsigned int shader = glCreateShader(type);
        glShaderSource(shader, 1, &shaderCode, nullptr);
        glCompileShader(shader);
        checkCompileErrors(shader, type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT");
        return shader;
    }

    void Shader::checkCompileErrors(GLuint shader, std::string type) {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }

    void Shader::use() {
        std::cout << "Using shader: " << ID << std::endl;
        glUseProgram(ID);
    }
    void Shader::dispose() {
        glUseProgram(0);
    }


    // Utility uniform functions
    void Shader::setBool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }

    void Shader::setInt(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void Shader::setFloat(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    void Shader::setVec2(const std::string &name, const glm::vec2 &value) const {
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }

    void Shader::setVec4(const std::string &name, const glm::vec4 &value) const {
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
}
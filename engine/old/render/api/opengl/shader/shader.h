//
// Created by Administrator on 5/17/2024.
//

#ifndef SHADER_H
#define SHADER_H
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>


namespace backend::renderapi::opengl {
class Shader {
public:
    unsigned int ID; // Shader program ID

    // Constructors to initialize the shader from binary data or file paths
    Shader(const std::vector<unsigned char>& vertexShaderCode, const std::vector<unsigned char>& fragmentShaderCode);
    Shader(const std::string& vertexPath, const std::string& fragmentPath);

    // Activate the shader program
    void use();
    void dispose();

    // Uniform utility methods
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec2(const std::string &name, const glm::vec2 &value) const;
    void setVec4(const std::string &name, const glm::vec4 &value) const;

private:
    // Helper methods for shader compilation and linking
    void initialize(const char* vertexSource, const char* fragmentSource);
    unsigned int compileShader(const char* shaderCode, GLenum type);
    void checkCompileErrors(GLuint shader, std::string type);
    std::string readFile(const std::string& filePath);
};
}


#endif //SHADER_H

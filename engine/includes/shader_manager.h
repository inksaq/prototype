#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <GL/glew.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <span>

class ShaderManager {
public:
    ShaderManager();
    ~ShaderManager();

    GLuint loadShaderFromMemory(const std::span<const unsigned char>& vertexShaderData, 
                                const std::span<const unsigned char>& fragmentShaderData, 
                                const std::string& shaderName);
    GLuint getShader(const std::string& shaderName) const;
    void deleteShader(const std::string& shaderName);

private:
    std::unordered_map<std::string, GLuint> shaders;
    GLuint compileShaderFromMemory(const std::span<const unsigned char>& shaderData, GLenum shaderType) const;
    GLuint linkProgram(GLuint vertexShaderID, GLuint fragmentShaderID) const;
};

#endif // SHADER_MANAGER_H


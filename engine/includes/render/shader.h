#ifndef ENGINE_SHADER_H
#define ENGINE_SHADER_H

#include <span>
#include <GL\glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <unordered_map>

namespace Core::Render {

    class Shader {
    public:
        // Program ID
        GLuint ID{};

        // Constructors
        Shader(const std::vector<unsigned char>& vertexShaderCode,
           const std::vector<unsigned char>& fragmentShaderCode);
        Shader(const std::string& vertexPath, const std::string& fragmentPath);
        // New constructor for binary data
        Shader(const std::span<const unsigned char>& vertexShaderData,
               const std::span<const unsigned char>& fragmentShaderData);
        ~Shader();

        // Prevent copying
        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        // Allow moving
        Shader(Shader&& other) noexcept;
        Shader& operator=(Shader&& other) noexcept;

        // Activate/Deactivate shader
        void use();
        void dispose();

        // Utility uniform functions
        void setBool(const std::string& name, bool value) const;
        void setInt(const std::string& name, int value) const;
        void setFloat(const std::string& name, float value) const;
        void setVec2(const std::string& name, const glm::vec2& value) const;
        void setVec3(const std::string& name, const glm::vec3& value) const;
        void setVec4(const std::string& name, const glm::vec4& value) const;
        void setMat2(const std::string& name, const glm::mat2& mat) const;
        void setMat3(const std::string& name, const glm::mat3& mat) const;
        void setMat4(const std::string& name, const glm::mat4& mat) const;

        // Texture specific utilities
        void setTexture(const std::string& name, GLuint textureUnit) const;
        void bindTexture(GLuint textureID, GLuint unit) const;

    private:
        // Cache for uniform locations
        mutable std::unordered_map<std::string, GLint> uniformLocationCache;

        void initialize(const char* vertexSource, const char* fragmentSource);
        void initializeFromBinary(const std::span<const unsigned char>& vertexData,
                                const std::span<const unsigned char>& fragmentData);
        GLuint compileShader(const char* source, GLenum type);
        GLuint compileShaderFromMemory(const std::span<const unsigned char>& shaderData,
                                      GLenum shaderType) const;
        GLuint linkProgram(GLuint vertexShaderID, GLuint fragmentShaderID) const;
        void checkCompileErrors(GLuint shader, const std::string& type);
        GLint getUniformLocation(const std::string& name) const;
        std::string readFile(const std::string& filePath);
    };

} // namespace Core::Render

#endif // ENGINE_SHADER_H

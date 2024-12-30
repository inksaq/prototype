#ifndef ENGINE_TEXTURE_H
#define ENGINE_TEXTURE_H

#include <GL\glew.h>
#include <string>
#include <memory>

namespace Core::Render {
    struct TextureParameters {
        GLenum target = GL_TEXTURE_2D;
        GLenum minFilter = GL_LINEAR_MIPMAP_LINEAR;
        GLenum magFilter = GL_LINEAR;
        GLenum wrapS = GL_REPEAT;
        GLenum wrapT = GL_REPEAT;
        bool generateMipmap = true;
    };


class Texture {

public:
    // Constructors
    explicit Texture(const std::string& path,
                    const TextureParameters& params = TextureParameters());
    Texture(const unsigned char* data, int width, int height, int channels,
            const TextureParameters& params = TextureParameters());
    ~Texture();

    // Prevent copying
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    // Allow moving
    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    // Core functionality
    void bind(unsigned int unit = 0) const;
    void unbind() const;

    // Getters
    GLuint getID() const { return textureID; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getChannels() const { return channels; }

private:
    GLuint textureID;
    int width;
    int height;
    int channels;
    GLenum target;

    void initialize(const unsigned char* data,
                   const TextureParameters& params);
    void setParameters(const TextureParameters& params);
};

}
#endif
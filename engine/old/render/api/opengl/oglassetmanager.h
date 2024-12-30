//
// Created by Administrator on 5/22/2024.
//

#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <unordered_map>
#include <string>
#include <memory>
#include "shader/shader.h"
#include "texture/texture.h"
#include "buffer/buffer.h"
#include "buffer/frame.h"
#include "buffer/buffer.h"
#include "buffer/vertexarray.h"
#include "framebuffer/framebuffer.h"
#include "vertexarray/vertexarray.h"

namespace backend::renderapi::opengl {
    class OGLAssetManager {
    public:
        static OGLAssetManager& getInstance();

        std::shared_ptr<Shader> loadShader(const std::string& vertexPath, const std::string& fragmentPath);
        std::shared_ptr<Texture> loadTexture(const std::string& path);
        std::shared_ptr<Buffer> loadBuffer(const std::vector<float>& data, GLenum usage, GLenum target = GL_ARRAY_BUFFER);
        std::shared_ptr<Buffer> loadBuffer(const std::vector<uint32_t>& data, GLenum usage, GLenum target = GL_ELEMENT_ARRAY_BUFFER);
        std::shared_ptr<Framebuffer> createFramebuffer(int width, int height);
        std::shared_ptr<VertexArray> createVertexArray();

        std::shared_ptr<Shader> getShader(const std::string& name);
        std::shared_ptr<Texture> getTexture(const std::string& name);
        std::shared_ptr<Framebuffer> getFramebuffer(const std::string& name);
        std::shared_ptr<VertexArray> getVertexArray(const std::string& name);

    private:
        OGLAssetManager() = default;
        ~OGLAssetManager() = default;

        std::unordered_map<std::string, std::shared_ptr<Shader>> shaders_;
        std::unordered_map<std::string, std::shared_ptr<Texture>> textures_;
        std::unordered_map<std::string, std::shared_ptr<Buffer>> buffers_;
        std::unordered_map<std::string, std::shared_ptr<Framebuffer>> framebuffers_;
        std::unordered_map<std::string, std::shared_ptr<VertexArray>> vertexArrays_;
    };
}

#endif // ASSETMANAGER_H

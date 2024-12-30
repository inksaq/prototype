//
// Created by Administrator on 5/22/2024.
//

#include "oglassetmanager.h"
#include <sstream>

namespace backend::renderapi::opengl {
    OGLAssetManager& OGLAssetManager::getInstance() {
        static OGLAssetManager instance;
        return instance;
    }

    std::shared_ptr<Shader> OGLAssetManager::loadShader(const std::string& vertexPath, const std::string& fragmentPath) {
        std::string name = vertexPath + fragmentPath;
        auto it = shaders_.find(name);
        if (it != shaders_.end()) {
            return it->second;
        }
        auto shader = std::make_shared<Shader>(vertexPath, fragmentPath);
        shaders_[name] = shader;
        return shader;
    }

    std::shared_ptr<Texture> OGLAssetManager::loadTexture(const std::string& path) {
        auto it = textures_.find(path);
        if (it != textures_.end()) {
            return it->second;
        }
        auto texture = std::make_shared<Texture>(path);
        textures_[path] = texture;
        return texture;
    }

    std::shared_ptr<Buffer> OGLAssetManager::loadBuffer(const std::vector<float>& data, GLenum usage, GLenum target) {
        std::stringstream ss;
        ss << target << data.data();
        std::string name = ss.str();
        auto it = buffers_.find(name);
        if (it != buffers_.end()) {
            return it->second;
        }
        auto buffer = std::make_shared<Buffer>(data, usage, target);
        buffers_[name] = buffer;
        return buffer;
    }

    std::shared_ptr<Buffer> OGLAssetManager::loadBuffer(const std::vector<uint32_t>& data, GLenum usage, GLenum target) {
        std::stringstream ss;
        ss << target << data.data();
        std::string name = ss.str();
        auto it = buffers_.find(name);
        if (it != buffers_.end()) {
            return it->second;
        }
        auto buffer = std::make_shared<Buffer>(data, usage, target);
        buffers_[name] = buffer;
        return buffer;
    }

    std::shared_ptr<Framebuffer> OGLAssetManager::createFramebuffer(int width, int height) {
        std::stringstream ss;
        ss << width << "x" << height;
        std::string name = ss.str();
        auto it = framebuffers_.find(name);
        if (it != framebuffers_.end()) {
            return it->second;
        }
        auto framebuffer = std::make_shared<Framebuffer>(width, height);
        framebuffers_[name] = framebuffer;
        return framebuffer;
    }

    std::shared_ptr<VertexArray> OGLAssetManager::createVertexArray() {
        auto vertexArray = std::make_shared<VertexArray>();
        std::string name = std::to_string(reinterpret_cast<uintptr_t>(vertexArray.get()));
        vertexArrays_[name] = vertexArray;
        return vertexArray;
    }

    std::shared_ptr<Shader> OGLAssetManager::getShader(const std::string& name) {
        return shaders_.at(name);
    }

    std::shared_ptr<Texture> OGLAssetManager::getTexture(const std::string& name) {
        return textures_.at(name);
    }

    std::shared_ptr<Buffer> OGLAssetManager::getBuffer(const std::string& name) {
        return buffers_.at(name);
    }

    std::shared_ptr<Framebuffer> OGLAssetManager::getFramebuffer(const std::string& name) {
        return framebuffers_.at(name);
    }

    std::shared_ptr<VertexArray> OGLAssetManager::getVertexArray(const std::string& name) {
        return vertexArrays_.at(name);
    }
}

//
// Created by Administrator on 5/20/2024.
//

#include "frame.h"

namespace backend::renderapi::opengl {
    Framebuffer::Framebuffer(int width, int height) : width(width), height(height) {
        createFramebuffer();
    }

    void Framebuffer::createFramebuffer() {
        glGenFramebuffers(1, &ID);
        bind();

        createColorAttachment();
        createDepthAttachment();

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        }
        unbind();
    }

    void Framebuffer::createColorAttachment() {
        glGenTextures(1, &colorTextureID);
        glBindTexture(GL_TEXTURE_2D, colorTextureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTextureID, 0);
    }

    void Framebuffer::createDepthAttachment() {
        glGenTextures(1, &depthTextureID);
        glBindTexture(GL_TEXTURE_2D, depthTextureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTextureID, 0);
    }

    void Framebuffer::bind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, ID);
    }

    void Framebuffer::unbind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::bindRead() const {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, ID);
    }

    void Framebuffer::bindDraw() const {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, ID);
    }

    void Framebuffer::resize(int width, int height) {
        this->width = width;
        this->height = height;
        glBindTexture(GL_TEXTURE_2D, colorTextureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glBindTexture(GL_TEXTURE_2D, depthTextureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        unbind();
    }

    Framebuffer::~Framebuffer() {
        glDeleteFramebuffers(1, &ID);
        glDeleteTextures(1, &colorTextureID);
        glDeleteTextures(1, &depthTextureID);
    }
}

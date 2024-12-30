//
// Created by Administrator on 5/19/2024.
//

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H


#include <glad/gl.h>
#include <iostream>

namespace backend::renderapi::opengl {
    class Framebuffer {
    public:
        Framebuffer(int width, int height);
        void bind() const;
        void unbind() const;
        void bindRead() const;
        void bindDraw() const;
        void resize(int width, int height);
        ~Framebuffer();

        GLuint getID() const { return ID; }
        int getWidth() const { return width; }
        int getHeight() const { return height; }

    private:
        GLuint ID;
        GLuint colorTextureID;
        GLuint depthTextureID;
        int width;
        int height;

        void createFramebuffer();
        void createColorAttachment();
        void createDepthAttachment();
    };
}




#endif //FRAMEBUFFER_H

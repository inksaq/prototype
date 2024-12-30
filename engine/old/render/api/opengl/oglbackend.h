//
// Created by Administrator on 5/19/2024.
//

#ifndef OGLBACKEND_H
#define OGLBACKEND_H
#include <unordered_map>
#include <api/renderapi.h>

#include "buffer/buffer.h"
#include "buffer/frame.h"
#include "buffer/vertexarray.h"
#include "shader/shader.h"
#include "texture/texture.h"


namespace Backend {
    class OGLBackend : public RenderAPI {
    public:
        void init() override;
        void shutdown() override;

        void loadShaders();
        void loadBuffers();
        void loadTextures();
        void initFramebuffer();

        void UploadVertexData(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
        void renderLoadingScreen();
        void renderUI();

    private:
        std::unordered_map<unsigned int, backend::renderapi::opengl::Shader> shaders_;
        std::unordered_map<unsigned int, backend::renderapi::opengl::Framebuffer> buffers_;
        std::unordered_map<unsigned int, backend::renderapi::opengl::Texture> textures_;
        std::unordered_map<unsigned int, backend::renderapi::opengl::Framebuffer> framebuffers_;
        std::unordered_map<unsigned int, backend::renderapi::opengl::VertexArray> vertexArrays_;
    };
}


#endif //OGLBACKEND_H

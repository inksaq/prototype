//
// Created by Administrator on 5/17/2024.
//

#ifndef RENDERAPI_H
#define RENDERAPI_H

#include <string>
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>
#include <util/constants.hpp>

enum class BackendAPI {OPENGL,VULKAN,DIRECTX,METAL,UNDEFINED};
enum class WindowMode {WINDOWED, FULLSCREEN, BORDERLESS};


namespace Backend {

    class RenderAPI {
    public:
        RenderAPI() = default;
        virtual ~RenderAPI() = default;
        virtual void init() = 0;
        virtual void shutdown() = 0;

        // Create a shader program with given vertex and fragment shader source codes
        virtual unsigned int createShader(const char* vertex, const char* fragment) = 0;

        // Retrieve a pointer to the shader program by its ID
        virtual void* getShader(unsigned int id) = 0;

        // Destroy the shader program identified by its ID
        virtual void destroyShader(unsigned int id) = 0;

    };


}

#endif //RENDERAPI_H

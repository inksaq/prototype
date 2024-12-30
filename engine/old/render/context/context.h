//
// Created by Administrator on 5/19/2024.
//

#ifndef CONTEXT_H
#define CONTEXT_H
#include <mutex>
#include <glm/vec2.hpp>

#include <util.h>

#include <string>


namespace backend {
    class Logger;
    class context {
    public:
        std::string vendorname, rendererName, glVersion, glslVersion;
        uint32_t displayRefreshRate;
        glm::ivec2 displaySize;
        std::mutex logMutex;
        Context(uint32_t logLevel = INFO);
        ~Context();

        uint64_t registerWindow(Window* window);
        void makeCurrent();
        void makeWindowCurrent(uint64_t id);
        Window* getCurrentWindow();

        void pollEvents();
        void removeWindow(uint64_t id);
        void destroy();
        void loadGLFW();
        void loadGLAD();
    };
}




#endif //CONTEXT_H

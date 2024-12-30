//
// Created by Administrator on 5/19/2024.
//

#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <unordered_map>
#include <GLFW/glfw3.h>


namespace backend {
typedef void (*windowEvent_t)();
    class Window {


    public:


        Window(int32_t width, int32_t height, const char* title,bool fullscreen, bool vsync);
        ~Window();
        void loadGLAD();
        void loadGLFW();
        int32_t width, height;

        void update();
        void clear();
        void setEvent(std::string name, windowEvent_t func);
        void setVSync(bool vsync);
        void setTitle(const char* title);
        void getSize(int32_t* width, int32_t* height);

        GLFWwindow* getGLFWWindow();
        std::string getTitle();
        bool shouldClose();
        void swapBuffers();
        void pollEvents();
        void destroy();
        bool setFullscreen(bool fullscreen);
        bool isFullscreen();
        uint32_t getId();
        uint32_t getWidth();
        uint32_t getHeight();


        void handleKeyInput(int key, int scancode, int action, int mode);

    private:
        GLFWwindow* window;
        std::unordered_map<std::string, windowEvent_t> events;

        std::string title;
        uint32_t id;
    };
}


#endif //WINDOW_H

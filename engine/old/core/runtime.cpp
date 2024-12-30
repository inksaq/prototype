//
// Created by Administrator on 5/19/2024.
//

#include "runtime.h"

#include <constants.hpp>
#include <renderer.h>
#include <api/opengl/oglbackend.h>
#include <api/opengl/shader/shader.h>
#include <logger/logger.h>
#include <util/util.h>

#include "runtime.h"
#include "util/logger/logger.h"

namespace Engine {

    Runtime::Runtime(uint32_t logLevel) {
        // logger = new Util::Logger(logLevel);
        // audio = new Audio();
        // sceneManager = new SceneManager();
    }

    Runtime::~Runtime() {
        // delete logger;
        // delete audio;
        // delete sceneManager;
    }

    void Runtime::init() {
        // Determine the API based on platform/compatibility checks
        // Here, we assume OpenGL for simplicity
        renderAPI = BackendAPI::OPENGL;

        preloadRenderer();

        // Initialize other subsystems
        // audio->init();
        // sceneManager->init();
    }

    void Runtime::preloadRenderer() {
        Renderer::init(renderAPI);
    }

    void Runtime::render() {
        while (Renderer::windowIsOpen()) {
            Renderer::beginFrame();
            // Update and render logic for subsystems
            // sceneManager->update();
            // audio->update();
            Renderer::endFrame();
        }

        Renderer::cleanUp();
    }

    BackendAPI Runtime::getAPI() {
        return renderAPI;
    }

    Util::Logger* Runtime::getLogger() {
        return logger;
    }

    std::mutex& Runtime::getLogMutex() {
        return logMutex;
    }

} // namespace Engine


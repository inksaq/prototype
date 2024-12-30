//
// Created by Administrator on 5/19/2024.
//

#ifndef RUNTIME_H
#define RUNTIME_H
#include <constants.hpp>
#include <mutex>
#include <api/renderapi.h>
#include <logger/logger.h>


namespace Engine {
    class Runtime {
    public:
        Runtime(uint32_t logLevel = INFO);
        ~Runtime();

        void init();
        void render();

        BackendAPI getAPI();

        // Util::Logger* getLogger();
        // std::mutex& getLogMutex();

    private:
        void preloadRenderer();

        // Util::Logger* logger;
        // std::mutex logMutex;
        BackendAPI renderAPI;

        // Add pointers to other subsystems
        // Audio* audio;
        // SceneManager* sceneManager;
    };

}



#endif //RUNTIME_H

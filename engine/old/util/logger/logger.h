//
// Created by Administrator on 5/19/2024.
//

#ifndef LOGGER_H
#define LOGGER_H

#include <cstdint>
#include <iostream>
#include <ctime>
#include <vector>
#include <iomanip>
#include <sstream>
#include <string>
#include <GLFW/glfw3.h>

namespace Util {
    class Runtime;
    Runtime* getRunetimePointer();

void setTextAttribute(uint32_t color);
std::string strfttime(const char* format, const std::tm* time);
    class Logger {
    private:
        uint32_t level, limit;

    public:
        std::vector<std::string> logs;

        Logger();
        ~Logger();

        void log(uint32_t type, const char* message);
        std::vector<std::string>& getLogs();
        void setLevel(uint32_t level);
        uint32_t getLevel();
        void setLimit(uint32_t limit);
        uint32_t getLimit();
        void destroy();


    };
};




#endif //LOGGER_H

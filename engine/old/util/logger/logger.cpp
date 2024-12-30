//
// Created by Administrator on 5/19/2024.
//

#include "logger.h"

#include <constants.hpp>
#include <mutex>
namespace Util {
    void setTextAttribute(uint32_t color) {
#ifdef NICKEL2_PLATFORM_WINDOWS
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
#endif
    }

    std::string strftime(const char* format, const std::tm* time) {
        std::ostringstream os;
        os << std::put_time(time, format);
        return os.str();
    }

    Logger::Logger() {
        level = INFO, limit = 1024;
    }

    Util::Logger::~Logger() {

    }

    void Util::Logger::log(uint32_t type, const char* message) {
        if (level <= type) {
            std::time_t now;
            std::time(&now);
            std::tm* tmp = localtime(&now);
            if (logs.size() >= limit) logs.erase(logs.begin());
            const char* typeNames[] = {"info", "warning", "error", "fatal error"};
            uint32_t typeColors[] = {BRIGHT_GREEN, BRIGHT_MAGENTA, BRIGHT_RED, DARK_RED};
            uint32_t messageColors[] = {DARK_WHITE, BRIGHT_MAGENTA, BRIGHT_RED, DARK_RED};

            std::lock_guard<std::mutex> lock(Util::getRunetimePointer()->logMutex);
            logs.push_back(strftime("[%m/%d/%Y-%H:%M]: ", tmp) + "GRUNGE: " + std::string(typeNames[type]) + ": " + std::string(message));

            setTextAttribute(WHITE);
            std::cout << "GRUNGE: ";
            setTextAttribute(typeColors[type]);
            std::cout << typeNames[type] << ": ";
            setTextAttribute(messageColors[type]);
            std::cout << message << std::endl;
            setTextAttribute(WHITE);
        }
    }

    std::vector <std::string>& Logger::getLogs() {
        return logs;
    }

    void Logger::setLevel(uint32_t level) {
        this->level = level;
    }

    uint32_t Logger::getLevel() {
        return level;
    }

    void Logger::setLimit(uint32_t limit) {
        this->limit = limit;
    }

    uint32_t Logger::getLimit() {
        return limit;
    }

    void Logger::destroy() {

    }
}
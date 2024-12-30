//
// Created by Administrator on 12/31/2024.
//

#include <engine/includes/core/logger.h>
namespace Core::Engine {

    std::string strftime(const char* format, const std::tm* time) {
        std::ostringstream os;
        os << std::put_time(time, format);
        return os.str();
    }

    void SetTextAttribute(uint32_t color) {
        #ifdef WIN32
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
        #endif
    }

    std::vector<std::string> Logger::logs = {};
    std::mutex Logger::mutex = {};
    Logger::Level Logger::level = Logger::Level::Trace;
    uint32_t Logger::limit = 512;

    void Logger::init() {
        Logger::level = Logger::Level::Trace;
        Logger::limit = 512;
    }

    void Logger::terminate() {}

    void Logger::Log(Level level, const char* prefix, const char* message) {
      if (Logger::level <= level) {
        std::time_t now;
        std::time(&now);
        std::tm* time = std::localtime(&now);
        if (logs.size() >= Logger::limit) { logs.erase(logs.begin()); }
      }
    }


}
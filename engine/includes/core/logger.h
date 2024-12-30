//
// Created by Administrator on 12/31/2024.
//

#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <ctime>
#include <vector>
#include <iomanip>
#include <sstream>
#include <string>
#include <mutex>

namespace Core::Engine {
  std::string strftime(const char* format, const std::tm* time);
  void SetTextAttribute(uint32_t color);

class Logger {
  public:
    enum class Level : uint8_t {
      Trace = 0, Debug, Info, Warn, Error
    };
    static const char* LevelToString(Level level){
      switch (level) {
        case Level::Trace: return "Trace";
        case Level::Debug: return "Debug";
        case Level::Info: return "Info";
        case Level::Warn: return "Warn";
        case Level::Error: return "Error";
      }
    }
  private:
    static Level level;
    static uint32_t limit;
    static std::vector<std::string> logs;
    static std::mutex mutex;

    public:
      static void init();
      static void terminate();

      static void Log(Level level, const char* prefix, const char* message);
      static void Log(Level level, const char* message);
      static std::vector<std::string> getLogs();

      static void setLevel(Level level);
      static Level getLevel();

      static void setLimit(uint32_t limit);
      static uint32_t getLimit();


};

}

#endif //LOGGER_H

#include "Logger.h"
#include <iostream>
#include <memory>
#include <spdlog/common.h>
#include <spdlog/sinks/stdout_color_sinks.h>
// Constructor: Opens the log file in append mode
Logger::Logger(bool toStdOut) {
  myLogger->set_level(spdlog::level::debug);
  if (toStdOut) {
    myLogger->sinks().push_back(
        std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
  }
}

// Destructor: Closes the log file
Logger::~Logger() {}

Logger::Logger(Logger &&other) noexcept {}
Logger &Logger::operator=(Logger &&other) noexcept {
  if (this != &other) {
  }
  return *this;
}
template <typename T> void Logger::addSink(std::shared_ptr<T> sink) {
  try {
    myLogger->sinks().push_back(sink);
  } catch (std::exception &e) {
    std::cerr << "Error adding sink: " << e.what() << std::endl;
  }
}

// Logs a message with a given log level
void Logger::log(LogLevel level, const std::string &message) {
  myLogger->log(static_cast<spdlog::level::level_enum>(level), message);
}

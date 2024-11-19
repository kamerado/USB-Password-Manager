#include "Logger.h"
#include "spdlog/spdlog.h"
#include <iostream>
#include <memory>
#include <spdlog/common.h>
#include <spdlog/sinks/stdout_color_sinks.h>
// Constructor: Opens the log file in append mode
Logger::Logger(bool toStdOut) {
  std::vector<spdlog::sink_ptr> sinks;
  if (toStdOut) {
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
  }

  myLogger =
      std::make_shared<spdlog::logger>("myLogger", sinks.begin(), sinks.end());
  myLogger->set_level(spdlog::level::trace);
  myLogger->flush_on(spdlog::level::trace); // Force flush on every message


  // Set the log pattern to include line numbers
  myLogger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%s:%#] %v");
  // Example pattern explanation:
  // [%Y-%m-%d %H:%M:%S.%e] - Timestamp
  // [%^%l%$] - Log level with color
  // [%s:%#] - Source file name and line number
  // %v - The actual log message
  spdlog::set_default_logger(myLogger); // Set the default logger to use myLogger
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
  myLogger->flush();
}

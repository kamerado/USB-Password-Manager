#ifndef LOGGER_H
#define LOGGER_H

#include <memory>
#include <spdlog/common.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

// Enum to represent log levels
typedef enum {
  DEBUG = spdlog::level::debug,
  INFO = spdlog::level::info,
  WARNING = spdlog::level::warn,
  ERROR = spdlog::level::err,
  CRITICAL = spdlog::level::critical,
  TRACE = spdlog::level::trace
} LogLevel;

class Logger {
public:
  Logger(bool toStdOut = true);

  Logger(const Logger &) = delete;
  Logger &operator=(const Logger &) = delete;

  Logger(Logger &&other) noexcept;
  Logger &operator=(Logger &&other) noexcept;

  // Destructor: Closes the log file
  ~Logger();

  template <typename T> void addSink(std::shared_ptr<T> sink);

  // Logs a message with a given log level
  void log(LogLevel level, const std::string &message);

private:
  std::shared_ptr<spdlog::logger> myLogger;
};

#endif

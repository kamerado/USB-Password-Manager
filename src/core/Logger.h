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

  template<typename... Args>
  void inline debug(const char* file, int line, const char* fmt, const Args&... args) {
      myLogger->log(spdlog::source_loc{file, line, SPDLOG_FUNCTION}, spdlog::level::debug, fmt, args...);
  }


  template<typename... Args>
  void inline error(const char* file, int line, const char* fmt, const Args&... args) {
      myLogger->log(spdlog::source_loc{file, line, SPDLOG_FUNCTION}, spdlog::level::err, fmt, args...);
  }


  template<typename... Args>
  void inline info(const char* file, int line, const char* fmt, const Args&... args) {
      myLogger->log(spdlog::source_loc{file, line, SPDLOG_FUNCTION}, spdlog::level::info, fmt, args...);
  }

  template<typename... Args>
  void inline warn(const char* file, int line, const char* fmt, const Args&... args) {
      myLogger->log(spdlog::source_loc{file, line, SPDLOG_FUNCTION}, spdlog::level::warn, fmt, args...);
  }

  template<typename... Args>
  void inline critical(const char* file, int line, const char* fmt, const Args&... args) {
      myLogger->log(spdlog::source_loc{file, line, SPDLOG_FUNCTION}, spdlog::level::critical, fmt, args...);
  }

  template<typename... Args>
  void inline trace(const char* file, int line, const char* fmt, const Args&... args) {
      myLogger->log(spdlog::source_loc{file, line, SPDLOG_FUNCTION}, spdlog::level::trace, fmt, args...);
  }

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

// Define custom macros to include the source file and line number
#define LOG_DEBUG(logger, ...) logger->debug(__FILE__, __LINE__, __VA_ARGS__)
#define LOG_INFO(logger, ...) logger->info(__FILE__, __LINE__, __VA_ARGS__)
#define LOG_WARN(logger, ...) logger->warn(__FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(logger, ...) logger->error(__FILE__, __LINE__, __VA_ARGS__)
#define LOG_CRITICAL(logger, ...) logger->critical(__FILE__, __LINE__, __VA_ARGS__)
#define LOG_TRACE(logger, ...) logger->trace(__FILE__, __LINE__, __VA_ARGS__)

#endif

#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>

  
// Enum to represent log levels 
enum LogLevel { DEBUG, INFO, WARNING, ERROR, CRITICAL }; 
  
class Logger { 
public: 
    // Constructor: Opens the log file in append mode 
    Logger(); 
  
    // Destructor: Closes the log file 
    ~Logger();
  
    // Logs a message with a given log level 
    void log(LogLevel level, const std::string& message);
  
private: 
    std::ofstream logFile; // File stream for the log file 
  
    // Converts log level to a string for output 
    std::string levelToString(LogLevel level); 
};

#endif
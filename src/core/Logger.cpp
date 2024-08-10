#include <ctime> 
#include <fstream> 
#include <iostream> 
#include <sstream>

#include "Logger.h"
// Constructor: Opens the log file in append mode 
Logger::Logger() {
    // Get current timestamp 
    time_t now = time(0); 
    tm* timeinfo = localtime(&now); 
    char timestamp[20]; 
    strftime(timestamp, sizeof(timestamp), 
                "%Y-%m-%d %H:%M:%S", timeinfo); 

    // Create log entry 
    std::ostringstream logFilePath; 
    logFilePath << "../LogFiles/[" << timestamp << "]Log.txt" << std::endl;
        // Create log entry 
    std::ostringstream logFileName; 
    logFileName << "[" << timestamp << "]Log.txt" << std::endl;

    this->logFPath = logFilePath.str();
    this->logFName = logFileName.str();
} 

// Destructor: Closes the log file 
Logger::~Logger() { logFile.close(); } 

// Logs a message with a given log level 
void Logger::log(LogLevel level, const std::string& message) { 
    // Get current timestamp 
    time_t now = time(0); 
    tm* timeinfo = localtime(&now); 
    char timestamp[20]; 
    strftime(timestamp, sizeof(timestamp), 
                "%Y-%m-%d %H:%M:%S", timeinfo); 

    // Create log entry 
    std::ostringstream logEntry; 
    logEntry << "[" << timestamp << "] "
                << levelToString(level) << ": " << message 
                << std::endl;

    logFile.open(this->logFName, std::ios::app);
    if (!logFile.is_open()) { 
        std::cerr << "Error opening log file." << std::endl; 
    } 
    std::cout << "opened LogFile: " << this->logFName << "successfully." << std::endl;

    // Output to console 
    std::cout << logEntry.str(); 

    // Output to log file 
    if (logFile.is_open()) { 
        logFile << logEntry.str(); 
        logFile.flush(); // Ensure immediate write to file 
    } 
} 

// Converts log level to a string for output 
std::string Logger::levelToString(LogLevel level) { 
    switch (level) { 
    case DEBUG: 
        return "DEBUG"; 
    case INFO: 
        return "INFO"; 
    case WARNING: 
        return "WARNING"; 
    case ERROR: 
        return "ERROR"; 
    case CRITICAL: 
        return "CRITICAL"; 
    default: 
        return "UNKNOWN"; 
    } 
} 

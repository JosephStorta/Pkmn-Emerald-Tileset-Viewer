#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>

/**
 * @brief Simple enum that stores log severity.
 */
enum class LogLevel
{
    DEBUG_L,
    INFO_L,
    WARNING_L,
    ERROR_L
};

/**
 * @brief Helper class to log messages to the terminal.
 */
class Logger
{
public:
    static void log(LogLevel level, const std::string& module, const std::string& message);

private:
    static void log_debug(const std::string& module, const std::string& message);
    static void log_info(const std::string& module, const std::string& message);
    static void log_warning(const std::string& module, const std::string& message);
    static void log_error(const std::string& module, const std::string& message);
};

// tfw a C++ programmer uses macros

/** 
 * @brief Sets the module name to be used for log messages from this file.
 * @attention Must be set before using any other log macro.
 */
#define SET_LOG_MODULE(module) const std::string log_module { module }

/**
 * @brief Prints a debug message to the terminal.
 */
#define LOG_DEBUG(message) Logger::log(LogLevel::DEBUG_L, log_module, message)

/**
 * @brief Prints an info message to the terminal.
 */
#define LOG_INFO(message) Logger::log(LogLevel::INFO_L, log_module, message)

/**
 * @brief Prints a warning message to the terminal.
 */
#define LOG_WARNING(message) Logger::log(LogLevel::WARNING_L, log_module, message)

/**
 * @brief Prints an error message to the terminal.
 * @note Does not throw an exception like a standard error.
 */
#define LOG_ERROR(message) Logger::log(LogLevel::ERROR_L, log_module, message)

#endif // LOGGER_H_

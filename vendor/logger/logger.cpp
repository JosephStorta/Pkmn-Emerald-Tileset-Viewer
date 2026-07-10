#include "logger.h"

#include <iostream>
#include <string>

/**
 * @brief Prints a message to the terminal.
 * @param level The severity at which to print the log (Debug, Info, Warning, Error).
 * @param module The name of the module sending the log (Set using the SET_LOG_MODULE() macro).
 * @param message The message to print.
 * @note The message printed is in the format "[LEVEL] [MODULE] Message".
 */
void Logger::log(LogLevel level, const std::string& module, const std::string& message)
{
    switch (level)
    {
        case LogLevel::DEBUG_L:
            log_debug(module, message);
            break;

        case LogLevel::INFO_L:
            log_info(module, message);
            break;

        case LogLevel::WARNING_L:
            log_warning(module, message);
            break;

        case LogLevel::ERROR_L:
            log_error(module, message);
            break;
    }
}

// --- ANSI codes for terminal text formatting --- //
const std::string escape_code { "\x1b[" }; // Initial ANSI escape sequence
const std::string reset_code { "0m" }; // Format reset

const std::string debug_code { "36m" }; // Cyan
const std::string info_code { "39m" }; // White
const std::string warning_code { "33m" }; // Yellow
const std::string error_code { "1;31m" }; // Bold and Red

/**
 * @brief Handles printing a debug message.
 * @param module The name of the module making the log.
 * @param message The message to print.
 */
void Logger::log_debug(const std::string& module, const std::string& message)
{
    std::cout << escape_code << debug_code;
    std::cout << "[DEBUG] [" << module << "] " << message << std::endl;
    std::cout << escape_code << reset_code;
}

/**
 * @brief Handles printing an info message.
 * @param module The name of the module making the log.
 * @param message The message to print.
 */
void Logger::log_info(const std::string& module, const std::string& message)
{
    std::cout << escape_code << info_code;
    std::cout << "[INFO] [" << module << "] " << message << std::endl;
    std::cout << escape_code << reset_code;
}

/**
 * @brief Handles printing a warning message.
 * @param module The name of the module making the log.
 * @param message The message to print.
 */
void Logger::log_warning(const std::string& module, const std::string& message)
{
    std::cout << escape_code << warning_code;
    std::cout << "[WARNING] [" << module << "] " << message << std::endl;
    std::cout << escape_code << reset_code;
}

/**
 * @brief Handles printing an error message.
 * @param module The name of the module making the log.
 * @param message The message to print.
 */
void Logger::log_error(const std::string& module, const std::string& message)
{
    std::cout << escape_code << error_code;
    std::cerr << "[ERROR] [" << module << "] " << message << std::endl;
    std::cout << escape_code << reset_code;
}

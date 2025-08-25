#pragma once
#include <iostream>
#include <ostream>
#include <string>

enum class LogLevel
{
    FatalError, Error, Warning, Info
};

class Logger
{
public:

    template <typename T>
    static void Log(LogLevel level, const std::string& message)
    {        
        switch (level)
        {
        case LogLevel::FatalError:
            std::cout << "\033[1;31m" << "FATAL::";  // Red Bold
            break;
        case LogLevel::Error:
            std::cout << "\033[0;31m" << "ERROR::";  // Red
            break;
        case LogLevel::Warning:
            std::cout << "\033[0;33m" << "WARNING::"; // Yellow
            break;
        case LogLevel::Info:
            std::cout << "\033[0;36m" << "INFO::"; // Cyan
            break;
        }

        std::cout << typeid(T).name() << "::" << message << "\033[0m" << std::endl;

        if (level == LogLevel::FatalError)
        {
            std::exit(EXIT_FAILURE);
        }
    }

    static void Log(LogLevel level, const std::string& message)
    {
        switch (level)
        {
        case LogLevel::FatalError:
            std::cout << "\033[1;31m" << "FATAL::";  // Red Bold
            break;
        case LogLevel::Error:
            std::cout << "\033[0;31m" << "ERROR::";  // Red
            break;
        case LogLevel::Warning:
            std::cout << "\033[0;33m" << "WARNING::"; // Yellow
            break;
        case LogLevel::Info:
            std::cout << "\033[0;36m" << "INFO::"; // Cyan
            break;
        }

        std::cout << message << "\033[0m" << std::endl;

        
        if (level == LogLevel::FatalError)
        {
            throw std::runtime_error(message);
        }
    }
    
};

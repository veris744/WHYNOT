#pragma once
#include <iostream>
#include <ostream>
#include <string>

#include "Managers/Helper.h"

using std::cout;

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

        cout << typeid(T).name() << "::" << message << "\033[0m" << std::endl; 

        if (level == LogLevel::FatalError)
        {
            glfwTerminate();
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
            glfwTerminate();
            throw std::runtime_error(message);
        }
    }
    
};

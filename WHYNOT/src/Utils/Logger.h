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
            cout << "\033[1;31m" << "FATAL::";
            break;
        case LogLevel::Error:
            cout << "\033[31m" << "ERROR::";
            break;
        case LogLevel::Warning:
            cout << "\033[33m" << "WARNING::";
            break;
        case LogLevel::Info:
            cout << "\033[37m" << "INFO::";
            break;
        }
        cout << typeid(T).name() << "::" << message << "\033[37m" << std::endl;

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
            cout << "\031[" << "FATAL::";
            break;
        case LogLevel::Error:
            cout << "\031[" << "ERROR::";
            break;
        case LogLevel::Warning:
            cout << "\033[" << "WARNING::";
            break;
        case LogLevel::Info:
            cout << "\037[" << "INFO::";
            break;
        }
        cout << message << "\033[0m" << std::endl;

        
        if (level == LogLevel::FatalError)
        {
            glfwTerminate();
            throw std::runtime_error(message);
        }
    }
    
};

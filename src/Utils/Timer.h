#pragma once
#include <thread>

namespace Timer
{
    static void StartTimer(float seconds, void (*memberFunc)()) {
        int milliseconds = static_cast<int>(seconds * 1000);
        std::thread([=]() {
           std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
           memberFunc();
       }).detach();
    }
    
    template <typename... Args>
    static void StartTimer(float seconds, void (*memberFunc)(Args...), Args... args) {
        int milliseconds = static_cast<int>(seconds * 1000);
        std::thread([=]() {
           std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
           memberFunc(args...);
       }).detach();
    }
    
    template <typename T>
    static void StartTimer(float seconds, T* instance, void (T::*memberFunc)()) {
        int milliseconds = static_cast<int>(seconds * 1000);
        std::thread([=]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
            (instance->*memberFunc)();
        }).detach();
    }
    
    template <typename T, typename... Args>
    static void StartTimer(float seconds, T* instance, void (T::*memberFunc)(Args...), Args... args) {
        int milliseconds = static_cast<int>(seconds * 1000);
        std::thread([=]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
            (instance->*memberFunc)(args...);
        }).detach();
    }
}


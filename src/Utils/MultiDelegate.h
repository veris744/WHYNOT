#pragma once
#include <functional>
#include <vector>

using std::vector;

template<typename ... Args>
class MultiDelegate
{
public:
    using Callback = std::function<void(Args...)>;

private:
    std::vector<std::pair<Callback, void*>> callbacks;
    
public:

    template <typename T>
    void Add(void (T::*memberFunc)(Args...), T* instance) {
        callbacks.emplace_back(
            [memberFunc, instance](Args... args) {
                (instance->*memberFunc)(args...);
            },
            static_cast<void*>(instance)
        );
    }
    
    template <typename T>
    void Remove(T* instance) {
        auto it = std::find_if(callbacks.begin(), callbacks.end(),
            [instance](const auto& entry) {
                void* storedInstance = entry.second;
                return storedInstance == static_cast<void*>(instance);
            });

        if (it != callbacks.end()) {  // Check if instance exists
            callbacks.erase(it);  // Remove the element if it exists
        }
    }


    void Clear() {
        callbacks.clear();
    }

    void Execute(Args... args) const {
        for (const auto& callback : callbacks) {
            if (!callback.first)
            {
                continue;
            }
            callback.first(args...);
            if (callbacks.empty())
            {
                break;
            }
        }
    }
    
};

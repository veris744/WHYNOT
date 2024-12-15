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
    vector<Callback> callbacks;
    
public:

    void Add(Callback _callback) {
        callbacks.push_back(std::move(_callback));
    }

    template <typename T>
    void Add(void (T::*memberFunc)(Args...), T* instance) {
        callbacks.push_back([memberFunc, instance](Args... args) {
            (instance->*memberFunc)(args...);
        });
    }

    void Remove(Callback _callback) {
        callbacks.erase(
            std::remove_if(callbacks.begin(), callbacks.end(),
                           [&_callback](const Callback& existing) {
                               return existing.target_type() == _callback.target_type();
                           }),
            callbacks.end()
        );
    }

    void Clear() {
        callbacks.clear();
    }

    void Execute(Args... args) const {
        for (const auto& callback : callbacks)
        {
            if (callback) callback(std::forward<Args>(args)...);
        }
    }
    
};

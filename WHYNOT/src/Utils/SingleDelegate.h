#pragma once
#include <functional>
#include <vector>

using std::vector;

template<typename ... Args>
class SingleDelegate
{
public:
    using Callback = std::function<void(Args...)>;

private:
    Callback callback;
    
public:

    void Bind(Callback cb) {
        callback = std::move(cb);
    }

    template <typename T>
    void Bind(void (T::*memberFunc)(Args...), T* instance) {
        callback = [memberFunc, instance](Args... args) {
            (instance->*memberFunc)(args...);
        };
    }

    void Execute(Args... args) const {
        if (callback) callback(std::forward<Args>(args)...);
    }

    void Clear()
    {
        callback = nullptr;
    }
};

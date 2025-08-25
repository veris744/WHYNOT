#pragma once
#include <string>
#include "Reflection/ReflectedObject.h"

class Entity;

class Component : public ReflectedObject
{
protected:
    bool isSkippedWhenPause = true;

public:
    std::string name;
    Component() = default;
    virtual ~Component() = default;

    std::string GetName()
    {
        return name;
    }
    
    Entity* parent;

    virtual void Update(float deltaTime) = 0;
    virtual void Initialize() {}
    virtual void RenderDebug() {}

    bool debugEnabled = false;
    bool IsSkippedWhenPause() const { return isSkippedWhenPause; }
    void SkipWhenPause(bool _skip) { isSkippedWhenPause = _skip; }
};

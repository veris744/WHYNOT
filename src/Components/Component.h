#pragma once
#include <string>
#include "Reflection/ReflectedObject.h"

using namespace std;

class Entity;

class Component : public ReflectedObject
{
protected:
    string name;
    
public:
    Component() = default;
    virtual ~Component() = default;

    string GetName()
    {
        return name;
    }
    
    Entity* parent;

    virtual void Update(float deltaTime) = 0;
    virtual void Initialize() {}
    virtual void RenderDebug() {}

    bool debugEnabled = false;
};

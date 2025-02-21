#pragma once
#include <includes.h>

class Entity;

class Component
{
protected:
    string name;
    
public:
    Component() = default;
    virtual ~Component()
    {
        //Logger::Log(LogLevel::Warning, "Component::~Component(): " + name);
    };

    string GetName()
    {
        return name;
    }
    
    Entity* parent;

    virtual void Update(float deltaTime) = 0;
    virtual void Initialize() {};
};

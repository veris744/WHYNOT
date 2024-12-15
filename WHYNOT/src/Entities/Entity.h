#pragma once
#include <includes.h>

#include "Components/Component.h"

class Component;
class Transform;

class Entity
{
    vector<std::shared_ptr<Component>> components = vector<std::shared_ptr<Component>>();
    bool isCamera = false;
    bool isLight = false;
    string name;
    
public:
    Entity(const string& _name, bool _isCamera = false, bool _isLight = false)
        : isCamera(_isCamera), isLight(_isLight), name(_name)
        {}
    ~Entity() = default;
    
    bool isRendered = false;
    
    template <typename T>
    void AddComponent(const std::shared_ptr<T>& _component)
    {
        if (GetComponent<T>())
        {
            Logger::Log<Entity>(LogLevel::Warning, "Entity " + name + " already has component " + _component->GetName());
            return;
        }
        _component->parent = this;
        components.push_back(_component);
    }

    template <typename T>
    const std::shared_ptr<T>& GetComponent()
    {
        for (const auto& component : components)
        {
            std::shared_ptr<T> derived = std::dynamic_pointer_cast<T>(component);
            if (derived)
            {
                return derived;
            }
        }
        return nullptr;
    }
    
    bool IsCamera() const { return isCamera; }
    bool IsLight() const { return isLight; }
    string GetName() { return name; }

    void Update(float deltaTime);
};


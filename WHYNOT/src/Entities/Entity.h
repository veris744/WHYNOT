#pragma once
#include <includes.h>

#include "Components/Component.h"

class Component;
class Transform;

class Entity : public std::enable_shared_from_this<Entity>
{    
protected:
    vector<std::shared_ptr<Component>> components = vector<std::shared_ptr<Component>>();
    bool isCamera = false;
    bool isLight = false;
    string name;

    Entity() = default;
    
public:
    Entity(const string& _name)
        :name(_name)
        {}
    Entity(const string& _name, bool _isCamera = false, bool _isLight = false)
        : isCamera(_isCamera), isLight(_isLight), name(_name)
        {}
    virtual ~Entity() = default;
    virtual void Initialize();
    
    bool isRendered = false;
    bool hasCollision = false;
    bool isActive = true;
    
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
    std::shared_ptr<T> GetComponent()
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

    void UpdateTrigger(float deltaTime);
    virtual void Update(float deltaTime);
    virtual void Destroy();
};


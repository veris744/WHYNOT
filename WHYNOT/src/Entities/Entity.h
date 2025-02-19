#pragma once
#include <includes.h>

#include "Components/Component.h"
#include "Reflection/Reflection.h"

class Model;
class Collider;
class LightSource;
class Camera;
class Component;
class Transform;

class Entity : public std::enable_shared_from_this<Entity>
{
    static unsigned int counter;
    
protected:
    vector<std::shared_ptr<Component>> components = vector<std::shared_ptr<Component>>();
    bool isCamera = false;
    bool isLight = false;
    bool isRendered = false;
    bool hasCollision = false;

    
public:
    Entity() = default;
    Entity(const string& _name)
        :name(_name)
        {}
    Entity(const string& _name, bool _isCamera = false, bool _isLight = false)
        : isCamera(_isCamera), isLight(_isLight), name(_name)
        {}
    virtual ~Entity() = default;
    virtual void Initialize();
    
    string name;
    bool isActive = true;
    
    template <typename T>
    void AddComponent(const std::shared_ptr<T>& _component)
    {
        if (GetComponent<T>())
        {
            Logger::Log<Entity>(LogLevel::Warning, "Entity " + name + " already has component " + _component->GetName());
            return;
        }
        if (is_base_of_v<Camera, T>)   isCamera = true;
        else if (is_base_of_v<LightSource, T>)   isLight = true;
        else if (is_base_of_v<Collider, T>)   hasCollision = true;
        else if (is_base_of_v<Model, T>)   isRendered = true;
        
        _component->parent = this;
        _component->Initialize();
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
    bool IsRendered() const { return isRendered; }
    bool HasCollision() const { return hasCollision; }
    
    string GetName() { return name; }
    virtual void SetAutoName();

    void UpdateTrigger(float deltaTime);
    virtual void Update(float deltaTime);
    virtual void Destroy();
    void ClearComponents()
    {
        components.clear();
    }
};
REGISTER_CLASS(Entity, {
    REGISTER_MEMBER(Entity, name)
})


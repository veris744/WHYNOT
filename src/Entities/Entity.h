#pragma once

#include "Components/Component.h"
#include "Reflection/Reflection.h"

class Model;
class Collider;
class LightSource;
class Camera;
class Component;
class Transform;

class Entity : public std::enable_shared_from_this<Entity>, public ReflectedObject
{
    static unsigned int counter;
    
protected:
    vector<std::unique_ptr<Component>> components = vector<std::unique_ptr<Component>>();
    bool isCamera = false;
    bool isLight = false;
    bool isRendered = false;
    bool hasCollision = false;

    
public:
    Entity() = default;
    Entity(const string& _name)
        :name(_name)
        {}
    Entity(const string& _name, bool _isCamera = false, bool _isLight = false, bool _isRendered = false, bool _hasCollision = false)
        : isCamera(_isCamera), isLight(_isLight), isRendered(_isRendered), hasCollision(_hasCollision), name(_name)
        {}
    virtual ~Entity() = default;
    
    virtual void Initialize();
    
    string name;
    bool isActive = true;
    bool debugEnabled = false;
    
    template <typename T>
    void AddComponent(std::unique_ptr<T> _component)
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
        components.push_back(std::move(_component));
    }

    template <typename T>
    T* GetComponent()
    {
        for (const auto& component : components)
        {
            T* derived = dynamic_cast<T*>(component.get());
            if (derived)
            {
                return derived;
            }
        }
        return nullptr;
    }

    const vector<std::unique_ptr<Component>>& GetComponents() const { return components; }
    
    bool IsCamera() const { return isCamera; }
    bool IsLight() const { return isLight; }
    bool IsRendered() const { return isRendered; }
    bool HasCollision() const { return hasCollision; }
    
    string GetName() { return name; }
    virtual void SetAutoName();

    void UpdateTrigger(float deltaTime);
    virtual void Update(float deltaTime);
    virtual void Destroy();
    virtual void ClearComponents()
    {
        components.clear();
    }

    virtual void OnOutOfBounds(vec3 _normal) {}
    virtual void OnCollision(Entity* _otherEntity, vec3 normal) {}
    virtual void OnClicked()
    {
        //Logger::Log<Entity>(LogLevel::Info, "Clicked on " + name);
    }
};
REGISTER_CLASS(Entity, {
    REGISTER_MEMBER(Entity, name, MemberProperty::Viewable),
    REGISTER_MEMBER(Entity, debugEnabled, MemberProperty::Viewable)
})


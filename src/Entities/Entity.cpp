#include "Entity.h"

#include <Components/PhysicsMaterial.h>

#include "Components/Component.h"
#include "Managers/World.h"

unsigned int Entity::counter = 0;

void Entity::Initialize()
{
    isActive = true;
    World::GetInstance()->AddEntity(shared_from_this());
}

PhysicsMaterial* Entity::GetPhysicsMaterial() const
{
    for (const auto& component : components)
    {
        PhysicsMaterial* derived = dynamic_cast<PhysicsMaterial*>(component.get());
        if (derived)
        {
            return derived;
        }
    }
    //Logger::Log<Entity>(LogLevel::Warning, "Returning Default Physics Material for " + name);
    return PhysicsMaterial::GetDefaultMaterial();
}


void Entity::SetAutoName()
{
    name = "Entity" + std::to_string(++counter);
}

void Entity::UpdateTrigger(float deltaTime)
{
    if (!isActive)  return;
    Update(deltaTime);
}

void Entity::Update(float deltaTime)
{
    debugEnabled = false;
    for (const auto& component : components)
    {
        if (component->debugEnabled)    debugEnabled = true;
        if (World::GetInstance()->IsPaused() && component->IsSkippedWhenPause())
            continue;

        component->Update(deltaTime);
    }
}

void Entity::Destroy()
{
    isActive = false;
    World::GetInstance()->MarkForDestruction(shared_from_this());
}

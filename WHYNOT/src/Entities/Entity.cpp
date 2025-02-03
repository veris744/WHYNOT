#include "Entity.h"

#include "Components/Component.h"
#include "Managers/World.h"


void Entity::Initialize()
{
    World::GetInstance()->AddEntity(shared_from_this());
}


void Entity::UpdateTrigger(float deltaTime)
{
    if (!isActive)  return;
    Update(deltaTime);
}

void Entity::Update(float deltaTime)
{
    for (const auto& component : components)
    {
        component->Update(deltaTime);
    }
}

void Entity::Destroy()
{
    World::GetInstance()->MarkForDestruction(name);
}

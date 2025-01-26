#include "Entity.h"

#include "Components/Component.h"
#include "Managers/World.h"


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

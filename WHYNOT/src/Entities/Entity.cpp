#include "Entity.h"

#include "Components/Component.h"


void Entity::Update(float deltaTime)
{
    for (const auto& component : components)
    {
        component->Update(deltaTime);
    }
}

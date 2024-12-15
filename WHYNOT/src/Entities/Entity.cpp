#include "Entity.h"

#include "Components/Component.h"


void Entity::Update()
{
    for (const auto& component : components)
    {
        component->Update();
    }
}

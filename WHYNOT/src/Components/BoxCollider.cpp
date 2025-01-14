#include "BoxCollider.h"

bool BoxCollider::Collides(const CircleCollider& other)
{
    return false;
}

bool BoxCollider::Collides(const BoxCollider& other)
{
    return false;
}

void BoxCollider::Render()
{
}

void BoxCollider::Update(float deltaTime)
{
}

#include "BoxCollider.h"

bool BoxCollider::Collides(const CircleCollider& other)
{
    return false;
}

bool BoxCollider::Collides(const BoxCollider& other)
{
    return false;
}

bool BoxCollider::CheckInBounds(const vec2& xBounds, const vec2& yBounds, const vec2& zBounds)
{
    return false;
}

void BoxCollider::Render()
{
}

void BoxCollider::Update(float deltaTime)
{
}

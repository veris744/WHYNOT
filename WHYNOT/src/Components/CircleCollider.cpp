#include "CircleCollider.h"

#include "BoxCollider.h"
#include "Managers/Renderer.h"
#include "Utils/Debugger.h"

bool CircleCollider::Collides(const CircleCollider& other)
{
    return CheckCircleCircle(radius, position, other.GetRadius(), other.position);
}

bool CircleCollider::Collides(const BoxCollider& other)
{
    return false;
}

void CircleCollider::Render()
{
    Debugger::DrawSphereDebug(radius, parent->GetComponent<Transform>()->v_position);    
}

void CircleCollider::Update(float deltaTime)
{
    position = parent->GetComponent<Transform>()->v_position;
}

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

bool CircleCollider::CheckInBounds()
{
    if (CheckCirclePlane(radius, position, vec3(-8, 0, 0), vec3(1, 0, 0)))
    {
        OnOutOfBoundsDelegate.Execute(vec3(1, 0, 0));
        return false;
    }
    if (CheckCirclePlane(radius, position, vec3(8, 0, 0), vec3(-1, 0, 0)))
    {
        OnOutOfBoundsDelegate.Execute(vec3(-1, 0, 0));
        return false;
    }
    if (CheckCirclePlane(radius, position, vec3(0, -8, 0), vec3(0, 1, 0)))
    {
        OnOutOfBoundsDelegate.Execute(vec3(0, 1, 0));
        return false;
    }
    if (CheckCirclePlane(radius, position, vec3(0, 8, 0), vec3(0, -1, 0)))
    {
        OnOutOfBoundsDelegate.Execute(vec3(0, -1, 0));
        return false;
    }
    if (CheckCirclePlane(radius, position, vec3(0, 0, 1), vec3(0, 0, 1)))
    {
        OnOutOfBoundsDelegate.Execute(vec3(0, 0, 1));
        return false;
    }
    if (CheckCirclePlane(radius, position, vec3(0, 0, 12), vec3(0, 0, -1)))
    {
        OnOutOfBoundsDelegate.Execute(vec3(0, 0, -1));
        return false;
    }
    return true;
}

void CircleCollider::Render()
{
    Debugger::DrawSphereDebug(radius, parent->GetComponent<Transform>()->v_position);    
}

void CircleCollider::Update(float deltaTime)
{
    position = parent->GetComponent<Transform>()->v_position;
}

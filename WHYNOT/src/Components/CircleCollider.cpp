#include "CircleCollider.h"

#include "BoxCollider.h"
#include "Managers/Renderer.h"
#include "Utils/Debugger.h"


bool CircleCollider::Collides(const std::shared_ptr<Collider>& other) 
{
    return other->Collides(radius, GetWorldPosition());
}

bool CircleCollider::Collides(float _rad1, vec3 _pos1)  
{
    return CheckCircleCircle(radius, GetWorldPosition(), _rad1, _pos1);
}

bool CircleCollider::CheckInBounds(const vec2& xBounds, const vec2& yBounds, const vec2& zBounds)
{
    if (CheckCirclePlane(radius, position, vec3(xBounds.x, 0, 0), vec3(1, 0, 0)))
    {
        OnOutOfBoundsDelegate.Execute(vec3(1, 0, 0));
        return false;
    }
    if (CheckCirclePlane(radius, position, vec3(xBounds.y, 0, 0), vec3(-1, 0, 0)))
    {
        OnOutOfBoundsDelegate.Execute(vec3(-1, 0, 0));
        return false;
    }
    if (CheckCirclePlane(radius, position, vec3(0, yBounds.x, 0), vec3(0, 1, 0)))
    {
        OnOutOfBoundsDelegate.Execute(vec3(0, 1, 0));
        return false;
    }
    if (CheckCirclePlane(radius, position, vec3(0, yBounds.y, 0), vec3(0, -1, 0)))
    {
        OnOutOfBoundsDelegate.Execute(vec3(0, -1, 0));
        return false;
    }
    if (CheckCirclePlane(radius, position, vec3(0, 0, zBounds.x), vec3(0, 0, 1)))
    {
        OnOutOfBoundsDelegate.Execute(vec3(0, 0, 1));
        return false;
    }
    if (CheckCirclePlane(radius, position, vec3(0, 0, zBounds.y), vec3(0, 0, -1)))
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

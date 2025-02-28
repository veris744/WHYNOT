#include "Collider.h"

#include "Entities/Entity.h"

vec3 Collider::GetWorldPosition()
{
    if (!transform)
    {
        transform = parent->GetComponent<Transform>();
    }
    return transform->position + position;
}

void Collider::Initialize()
{
    Component::Initialize();
    
    if (profile.mode == ColliderMode::All || profile.mode == ColliderMode::Query)
    {
        OnClickedDelegate.Bind(&Entity::OnClicked, parent);
    }
    if (profile.mode == ColliderMode::All)
    {
        OnOutOfBoundsDelegate.Bind(&Entity::OnOutOfBounds, parent);
        CollisionDelegate.Bind(&Entity::OnCollision, parent);
    }
}

bool Collider::CheckCircleCircle(float _rad1, vec3 _pos1, float _rad2, vec3 _pos2) const
{
    if (distance(_pos1, _pos2) <= _rad1 + _rad2)
    {
        return true;
    }
    return false;
}

bool Collider::CheckCirclePlane(float _rad1, vec3 _pos1, vec3 _point, vec3 _normal) const
{
    float distanceToPlane = dot(_normal, (_pos1 - _point));
    if (fabs(distanceToPlane) <= _rad1)
    {
        return true;
    }
    Logger::Log(LogLevel::Error, std::to_string(distanceToPlane));
    return false;
}

bool Collider::CheckCircleSquare(float _rad1, vec3 _pos1, vec3 _width2, vec3 _height2, vec3 _depth, vec3 _pos2) const
{
    vec3 halfExtents = vec3(_width2.x / 2.0f, _height2.y / 2.0f, _depth.z / 2.0f);

    vec3 squareMin = _pos2 - halfExtents;
    vec3 squareMax = _pos2 + halfExtents;

    vec3 closestPoint;
    closestPoint.x = std::max(squareMin.x, std::min(_pos1.x, squareMax.x));
    closestPoint.y = std::max(squareMin.y, std::min(_pos1.y, squareMax.y));
    closestPoint.z = std::max(squareMin.z, std::min(_pos1.z, squareMax.z));

    float distanceSquared = (closestPoint.x - _pos1.x) * (closestPoint.x - _pos1.x) +
        (closestPoint.y - _pos1.y) * (closestPoint.y - _pos1.y) +
        (closestPoint.z - _pos1.z) * (closestPoint.z - _pos1.z);

    return distanceSquared <= (_rad1 * _rad1);
}

bool Collider::CheckSquareSquare(vec3 _width1, vec3 _height1, vec3 _depth1, vec3 _pos1, vec3 _width2, vec3 _height2, vec3 _depth2, vec3 _pos2) const
{
    return false;
}




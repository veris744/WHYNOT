#include "Collider.h"

bool Collider::CheckCircleCircle(float _rad1, vec3 _pos1, float _rad2, vec3 _pos2)
{
    if (distance(_pos1, _pos2) <= (_rad1 + _rad2))
    {
        return true;
    }
    return false;
}

bool Collider::CheckCirclePlane(float _rad1, vec3 _pos1, vec3 _point, vec3 _normal)
{
    float distanceToPlane = dot(_normal, (_pos1 - _point));

    if (fabs(distanceToPlane) <= _rad1)
    {
        return true;
    }
    return false;
}

bool Collider::CheckCircleSquare(float _rad1, vec3 _pos1, vec3 _width2, vec3 _height2, vec3 _depth, vec3 _pos2)
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

bool Collider::CheckSquareSquare(vec3 _width1, vec3 _height1, vec3 _pos1, vec3 _width2, vec3 _height2, vec3 _pos2)
{
    return false;
}

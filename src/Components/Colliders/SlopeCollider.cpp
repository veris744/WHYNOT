#include "SlopeCollider.h"

#include <Utils/Debugger.h>

bool SlopeCollider::Collides(Collider* other, Hit& hit)
{
    other->Collides(dimensions, GetWorldPosition(), hit, true);
    if (hit.hasHit)
    {
        hit.otherEntity = other->parent;
        hit.selfEntity = parent;
        hit.type = WorldHit;
        vec3 directionToSelf = GetWorldPosition() - hit.point;
        if (dot(directionToSelf, hit.normal) < 0.0f)
        {
            hit.normal = -hit.normal;
        }
        return true;
    }
    return false;
}

bool SlopeCollider::Collides(float _rad1, vec3 _pos1, Hit& hit)
{
    return CheckSlopeCircle(dimensions, GetWorldPosition(), _rad1, _pos1, hit);
}

bool SlopeCollider::Collides(vec3 _dimensions, vec3 _pos1, Hit& hit, bool isSlope)
{
    if (!isSlope)
        return CheckSlopeSquare(dimensions, GetWorldPosition(), _dimensions, _pos1, hit);


    return CheckSlopeSlope(dimensions, GetWorldPosition(), _dimensions, _pos1, hit);
}

bool SlopeCollider::Collides(float _height, float _radius, vec3 _pos1, Hit& hit)
{
    return CheckSlopeCapsule(dimensions, GetWorldPosition(),_radius, _height, _pos1, hit);
}

bool SlopeCollider::Collides(vec2 _dimensions, vec3 _pos1, Hit& hit)
{
    return CheckSlopePlane(dimensions, GetWorldPosition(), _dimensions, _pos1, hit);
}

bool SlopeCollider::RayCollides(vec3 _rayOrigin, vec3 _rayDir, Hit& hit)
{
    vec3 boxCenter = GetWorldPosition();
    vec3 halfExtents = dimensions * 0.5f;
    vec3 boxMin = boxCenter - halfExtents;
    vec3 boxMax = boxCenter + halfExtents;

    // Check if the box is behind the ray origin
    vec3 toBox = boxCenter - _rayOrigin;
    float dotDir = dot(_rayDir, toBox);
    if (dotDir < 0)
        return false;

    float tMin = 0.0f;
    float tMax = FLT_MAX;
    vec3 normal;

    for (int i = 0; i < 3; ++i)
    {
        if (fabs(_rayDir[i]) < FLT_EPSILON) // Ray parallel to this axis
        {
            if (_rayOrigin[i] < boxMin[i] || _rayOrigin[i] > boxMax[i])
                return false; // No intersection if outside bounds
            continue;
        }

        float invDir = 1.0f / _rayDir[i];
        float t1 = (boxMin[i] - _rayOrigin[i]) * invDir;
        float t2 = (boxMax[i] - _rayOrigin[i]) * invDir;

        if (t1 > t2) std::swap(t1, t2); // Ensure t1 <= t2

        // Update tMin and normal only if this slab's entry is farther
        if (t1 > tMin)
        {
            tMin = t1;
            normal = vec3(0.0f);
            normal[i] = (_rayDir[i] > 0) ? -1.0f : 1.0f;
        }

        tMax = std::min(tMax, t2); // Update earliest exit

        if (tMin > tMax) return false; // No intersection
    }

    // Only register hits in front of the ray (tMin > 0)
    if (tMin > 0)
    {
        hit.distSQ = tMin * tMin; // Squared distance as intended
        hit.point = _rayOrigin + _rayDir * tMin;
        hit.normal = normal;
        hit.hasHit = true;
        hit.otherEntity = parent;
        hit.type = WorldHit;
        return true;
    }

    return false;
}

bool SlopeCollider::CheckInBounds(const vec2& xBounds, const vec2& yBounds, const vec2& zBounds, bool triggerDelegate)
{
    vec3 boxCenter = GetWorldPosition();
    vec3 halfExtents = dimensions * 0.5f;

    vec3 boxMin = boxCenter - halfExtents;
    vec3 boxMax = boxCenter + halfExtents;

    bool isInsideX = (boxMin.x >= xBounds.x) && (boxMax.x <= xBounds.y);
    bool isInsideY = (boxMin.y >= yBounds.x) && (boxMax.y <= yBounds.y);
    bool isInsideZ = (boxMin.z >= zBounds.x) && (boxMax.z <= zBounds.y);

    bool isCompletelyInside = isInsideX && isInsideY && isInsideZ;

    if (triggerDelegate && !isCompletelyInside)
    {
        OnOutOfBoundsDelegate.Execute(vec3(0));
    }

    return isCompletelyInside;
}

bool SlopeCollider::OverlapsBounds(const vec2& xBounds, const vec2& yBounds, const vec2& zBounds, bool triggerDelegate)
{
    vec3 boxCenter = GetWorldPosition();
    vec3 halfExtents = dimensions * 0.5f;

    // Calculate min and max extents of the box
    vec3 boxMin = boxCenter - halfExtents;
    vec3 boxMax = boxCenter + halfExtents;

    // Check for overlap on each axis
    bool xOverlap = (boxMax.x >= xBounds.x) && (boxMin.x <= xBounds.y);
    bool yOverlap = (boxMax.y >= yBounds.x) && (boxMin.y <= yBounds.y);
    bool zOverlap = (boxMax.z >= zBounds.x) && (boxMin.z <= zBounds.y);

    // The box overlaps if all axes overlap
    return xOverlap && yOverlap && zOverlap;
}

void SlopeCollider::RenderDebug()
{
    Debugger::DrawSlopeDebug(dimensions, GetWorldPosition());
}

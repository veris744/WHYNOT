#include "PlaneCollider.h"

#include <Utils/Debugger.h>

bool PlaneCollider::Collides(Collider* other, Hit& hit)
{
    other->Collides(dimensions, GetWorldPosition(), hit);
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

bool PlaneCollider::Collides(float _rad1, vec3 _pos1, Hit& hit)
{
    return CheckPlaneCircle(dimensions, GetWorldPosition(), {0, 1, 0}, _rad1,_pos1, hit);
}

bool PlaneCollider::Collides(vec3 _dimensions, vec3 _pos1, Hit& hit, bool isSlope)
{
    if (!isSlope)
        return CheckPlaneSquare(dimensions, GetWorldPosition(), {0, 1, 0}, _dimensions, _pos1, hit);

    return CheckSlopePlane(_dimensions, _pos1, dimensions, GetWorldPosition(), hit);
}

bool PlaneCollider::Collides(float _height, float _radius, vec3 _pos1, Hit& hit)
{
    return CheckPlaneCapsule(dimensions, GetWorldPosition(),{0, 1, 0}, _radius, _height, _pos1, hit);
}

bool PlaneCollider::Collides(vec2 _dimensions, vec3 _pos1, Hit& hit)
{
    return false;
}

bool PlaneCollider::RayCollides(vec3 _rayOrigin, vec3 _rayDir, Hit& hit)
{
    vec3 planeNormal = vec3(0,1,0);
    vec3 planeOrigin = GetWorldPosition();
    vec3 planeRight = vec3(1,0,0);
    vec3 planeUp = vec3(0,0,1);

    float halfWidth = dimensions.x * 0.5f;
    float halfHeight = dimensions.y * 0.5f;

    float denom = dot(planeNormal, _rayDir);
    if (fabs(denom) < FLT_EPSILON) return false;

    float t = dot(planeOrigin - _rayOrigin, planeNormal) / denom;
    if (t < 0) return false;

    vec3 hitPoint = _rayOrigin + _rayDir * t;

    // Convert hit point to plane-local space
    vec3 localHit = hitPoint - planeOrigin;
    float rightDist = dot(localHit, planeRight);
    float upDist = dot(localHit, planeUp);

    // Check if hit point is within plane bounds
    if (fabs(rightDist) > halfWidth || fabs(upDist) > halfHeight)
    {
        return false;
    }

    hit.hasHit = true;
    hit.point = hitPoint;
    hit.distSQ = dot(hitPoint - _rayOrigin, hitPoint - _rayOrigin);
    hit.normal = planeNormal;
    hit.otherEntity = parent;
    hit.type = WorldHit;

    return true;
}

bool PlaneCollider::CheckInBounds(const vec2& xBounds, const vec2& yBounds, const vec2& zBounds, bool triggerDelegate)
{

    // For finite planes (e.g., quad colliders)
    vec3 corners[4] = {
        GetWorldPosition() + vec3(1,0,0) * dimensions.x * 0.5f + vec3(0,1,0) * dimensions.y * 0.5f,
        GetWorldPosition() - vec3(1,0,0) * dimensions.x * 0.5f + vec3(0,1,0) * dimensions.y * 0.5f,
        GetWorldPosition() - vec3(1,0,0) * dimensions.x * 0.5f - vec3(0,1,0) * dimensions.y * 0.5f,
        GetWorldPosition() + vec3(1,0,0) * dimensions.x * 0.5f - vec3(0,1,0) * dimensions.y * 0.5f
    };

    for (const vec3& corner : corners) {
        if (corner.x < xBounds.x || corner.x > xBounds.y ||
            corner.y < yBounds.x || corner.y > yBounds.y ||
            corner.z < zBounds.x || corner.z > zBounds.y) {
            return false;
            }
    }

    if (triggerDelegate) {
        // OnBoundsFullyEnteredEvent.Invoke(this);
    }
    return true;
}

bool PlaneCollider::OverlapsBounds(const vec2& xBounds, const vec2& yBounds, const vec2& zBounds, bool triggerDelegate)
{
    // For finite planes - SAT (Separating Axis Theorem) test
    vec3 boundsCenter = {
        (xBounds.x + xBounds.y) * 0.5f,
        (yBounds.x + yBounds.y) * 0.5f,
        (zBounds.x + zBounds.y) * 0.5f
    };

    vec3 boundsExtents = {
        (xBounds.y - xBounds.x) * 0.5f,
        (yBounds.y - yBounds.x) * 0.5f,
        (zBounds.y - zBounds.x) * 0.5f
    };

    // Projection test
    vec3 planeNormal = vec3(0,1,0);
    float planeDistance = glm::dot(GetWorldPosition(), planeNormal);

    // Bounds extent along plane normal
    float r = boundsExtents.x * fabs(planeNormal.x) +
              boundsExtents.y * fabs(planeNormal.y) +
              boundsExtents.z * fabs(planeNormal.z);

    float s = glm::dot(planeNormal, boundsCenter) - planeDistance;

    if (fabs(s) > r) return false;

    if (triggerDelegate) {
        // OnBoundsOverlapEvent.Invoke(this);
    }
    return true;
}

void PlaneCollider::RenderDebug()
{
    Debugger::DrawPlaneDebug(dimensions, GetWorldPosition());
}

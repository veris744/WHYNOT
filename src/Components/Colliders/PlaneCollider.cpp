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
        glm::vec3 directionToSelf = GetWorldPosition() - hit.point;
        if (dot(directionToSelf, hit.normal) < 0.0f)
        {
            hit.normal = -hit.normal;
        }
        return true;
    }
    return false;
}

bool PlaneCollider::Collides(float _rad1, glm::vec3 _pos1, Hit& hit)
{
    return CheckPlaneCircle(dimensions, GetWorldPosition(), {0, 1, 0}, _rad1,_pos1, hit);
}

bool PlaneCollider::Collides(glm::vec3 _dimensions, glm::vec3 _pos1, Hit& hit, bool isSlope)
{
    if (!isSlope)
        return CheckPlaneSquare(dimensions, GetWorldPosition(), {0, 1, 0}, _dimensions, _pos1, hit);

    return CheckSlopePlane(_dimensions, _pos1, dimensions, GetWorldPosition(), hit);
}

bool PlaneCollider::Collides(float _height, float _radius, glm::vec3 _pos1, Hit& hit)
{
    return CheckPlaneCapsule(dimensions, GetWorldPosition(),{0, 1, 0}, _radius, _height, _pos1, hit);
}

bool PlaneCollider::Collides(glm::vec2 _dimensions, glm::vec3 _pos1, Hit& hit)
{
    return false;
}

bool PlaneCollider::RayCollides(glm::vec3 _rayOrigin, glm::vec3 _rayDir, Hit& hit)
{
    glm::vec3 planeNormal = glm::vec3(0,1,0);
    glm::vec3 planeOrigin = GetWorldPosition();
    glm::vec3 planeRight = glm::vec3(1,0,0);
    glm::vec3 planeUp = glm::vec3(0,0,1);

    float halfWidth = dimensions.x * 0.5f;
    float halfHeight = dimensions.y * 0.5f;

    float denom = dot(planeNormal, _rayDir);
    if (fabs(denom) < FLT_EPSILON) return false;

    float t = dot(planeOrigin - _rayOrigin, planeNormal) / denom;
    if (t < 0) return false;

    glm::vec3 hitPoint = _rayOrigin + _rayDir * t;

    // Convert hit point to plane-local space
    glm::vec3 localHit = hitPoint - planeOrigin;
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

bool PlaneCollider::CheckInBounds(const glm::vec2& xBounds, const glm::vec2& yBounds, const glm::vec2& zBounds)
{

    // For finite planes (e.g., quad colliders)
    glm::vec3 corners[4] = {
        GetWorldPosition() + glm::vec3(1,0,0) * dimensions.x * 0.5f + glm::vec3(0,1,0) * dimensions.y * 0.5f,
        GetWorldPosition() - glm::vec3(1,0,0) * dimensions.x * 0.5f + glm::vec3(0,1,0) * dimensions.y * 0.5f,
        GetWorldPosition() - glm::vec3(1,0,0) * dimensions.x * 0.5f - glm::vec3(0,1,0) * dimensions.y * 0.5f,
        GetWorldPosition() + glm::vec3(1,0,0) * dimensions.x * 0.5f - glm::vec3(0,1,0) * dimensions.y * 0.5f
    };

    for (const glm::vec3& corner : corners) {
        if (corner.x < xBounds.x || corner.x > xBounds.y ||
            corner.y < yBounds.x || corner.y > yBounds.y ||
            corner.z < zBounds.x || corner.z > zBounds.y) {
            return false;
            }
    }

    return true;
}

bool PlaneCollider::OverlapsBounds(const glm::vec2& xBounds, const glm::vec2& yBounds, const glm::vec2& zBounds)
{
    // For finite planes - SAT (Separating Axis Theorem) test
    glm::vec3 boundsCenter = {
        (xBounds.x + xBounds.y) * 0.5f,
        (yBounds.x + yBounds.y) * 0.5f,
        (zBounds.x + zBounds.y) * 0.5f
    };

    glm::vec3 boundsExtents = {
        (xBounds.y - xBounds.x) * 0.5f,
        (yBounds.y - yBounds.x) * 0.5f,
        (zBounds.y - zBounds.x) * 0.5f
    };

    // Projection test
    glm::vec3 planeNormal = glm::vec3(0,1,0);
    float planeDistance = glm::dot(GetWorldPosition(), planeNormal);

    // Bounds extent along plane normal
    float r = boundsExtents.x * fabs(planeNormal.x) +
              boundsExtents.y * fabs(planeNormal.y) +
              boundsExtents.z * fabs(planeNormal.z);

    float s = glm::dot(planeNormal, boundsCenter) - planeDistance;

    if (fabs(s) > r) return false;

    return true;
}

void PlaneCollider::RenderDebug()
{
    Debugger::DrawPlaneDebug(dimensions, GetWorldPosition());
}

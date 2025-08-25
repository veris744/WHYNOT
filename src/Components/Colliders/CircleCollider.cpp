#include "CircleCollider.h"

#include "BoxCollider.h"
#include "Entities/Entity.h"
#include "Utils/Debugger.h"


bool CircleCollider::Collides(Collider* other, Hit& hit)
{
    if (other->Collides(radius, GetWorldPosition(), hit))
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

bool CircleCollider::Collides(float _rad1, glm::vec3 _pos1, Hit& hit)
{
    return CheckCircleCircle(radius, GetWorldPosition(), _rad1, _pos1, hit);
}

bool CircleCollider::Collides(glm::vec3 _dimensions, glm::vec3 _pos1, Hit& hit, bool isSlope)
{
    if (!isSlope)
        return CheckCircleSquare(radius, GetWorldPosition(), _dimensions, _pos1, hit);

    return CheckSlopeCircle(_dimensions, _pos1, radius, GetWorldPosition(), hit);
}

bool CircleCollider::Collides(float _height, float _radius, glm::vec3 _pos1, Hit& hit)
{
    return CheckCapsuleCircle(_radius, _height, _pos1, radius, GetWorldPosition(), hit);
}

bool CircleCollider::Collides(glm::vec2 _dimensions, glm::vec3 _pos1, Hit& hit)
{
    return CheckPlaneCircle(_dimensions, _pos1, {0, 1, 0}, radius, GetWorldPosition(), hit);
}

// Collision from ray staring at origin extending towards infinity
bool CircleCollider::RayCollides(glm::vec3 _rayOrigin, glm::vec3 _rayDir, Hit& hit)
{
    glm::vec3 center = GetWorldPosition();
    glm::vec3 D = normalize(_rayDir);
    glm::vec3 OC = center - _rayOrigin;

    if (dot(OC, D) < 0) { 
        return false;
    }

    float b = dot(D, OC);
    float c = dot(OC, OC) - radius * radius;
    float discriminant = b * b - c;
    if (discriminant < 0.0f) {
        return false; // No intersection
    }

    // Compute the two possible intersection distances
    float sqrtDiscriminant = sqrt(discriminant);
    float t1 = b - sqrtDiscriminant; // Corrected t1
    float t2 = b + sqrtDiscriminant; // Corrected t2

    // Choose the smallest positive t (closest point in front of the ray)
    float t = -1.0f;
    if (t1 > 0) {
        t = t1;
    }
    if (t2 > 0 && (t == -1.0f || t2 < t)) {
        t = t2;
    }

    // If neither t1 nor t2 is valid (both are negative), return false
    if (t < 0) {
        return false;
    }

    // Calculate the hit point and the squared distance
    glm::vec3 hitPoint = _rayOrigin + t * D;
    float distSq = dot(hitPoint - _rayOrigin, hitPoint - _rayOrigin); // Squared distance

    hit.hasHit = true;
    hit.type = WorldHit;
    hit.distSQ = distSq;
    hit.point = hitPoint;
    hit.normal = normalize(hitPoint - center);
    hit.otherEntity = parent;

    return true;
}

bool CircleCollider::CheckInBounds(const glm::vec2& xBounds, const glm::vec2& yBounds, const glm::vec2& zBounds)
{
    bool isInside = true;
    glm::vec3 outNormal = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 worldPos = GetWorldPosition();

    // Check X bounds
    if (worldPos.x - radius < xBounds.x) {
        outNormal = glm::vec3(1.0f, 0.0f, 0.0f);
        isInside = false;
    } 
    else if (worldPos.x + radius > xBounds.y) {
        outNormal = glm::vec3(-1.0f, 0.0f, 0.0f);
        isInside = false;
    }

    // Check Y bounds
    if (worldPos.y - radius < yBounds.x) {
        outNormal = glm::vec3(0.0f, 1.0f, 0.0f);
        isInside = false;
    } 
    else if (worldPos.y + radius > yBounds.y) {
        outNormal = glm::vec3(0.0f, -1.0f, 0.0f);
        isInside = false;
    }

    // Check Z bounds
    if (worldPos.z - radius < zBounds.x) {
        outNormal = glm::vec3(0.0f, 0.0f, 1.0f);
        isInside = false;
    } 
    else if (worldPos.z + radius > zBounds.y) {
        outNormal = glm::vec3(0.0f, 0.0f, -1.0f);
        isInside = false;
    }

    return isInside;
}

bool CircleCollider::OverlapsBounds(const glm::vec2& xBounds, const glm::vec2& yBounds, const glm::vec2& zBounds)
{
    // Get the collider's center and radius
    glm::vec3 center = GetWorldPosition();

    // Check if the sphere overlaps with the AABB
    float closestX = std::clamp(center.x, xBounds.x, xBounds.y);
    float closestY = std::clamp(center.y, yBounds.x, yBounds.y);
    float closestZ = std::clamp(center.z, zBounds.x, zBounds.y);

    float distanceSquared = 
        (center.x - closestX) * (center.x - closestX) +
        (center.y - closestY) * (center.y - closestY) +
        (center.z - closestZ) * (center.z - closestZ);

    return distanceSquared <= (radius * radius);
}

void CircleCollider::RenderDebug()
{
    Debugger::DrawSphereDebug(radius, GetWorldPosition());
}





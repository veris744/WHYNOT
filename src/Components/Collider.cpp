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

bool Collider::CheckCircleCircle(float _rad1, vec3 _pos1, float _rad2, vec3 _pos2, Hit& hit) const
{
    hit.hasHit = false;
    vec3 delta = _pos2 - _pos1;
    float distanceSquared = dot(delta, delta);
    float combinedRadius = _rad1 + _rad2;
    float combinedRadiusSquared = combinedRadius * combinedRadius;

    if (distanceSquared <= combinedRadiusSquared)
    {
        float distance = sqrt(distanceSquared);
        hit.normal = (distance > 0.0001f) ? delta / distance : vec3(1.0f, 0.0f, 0.0f);
        float penetration = combinedRadius - distance;
        hit.point = _pos1 + hit.normal * (_rad1 - penetration * 0.5f);
        hit.distSQ = distanceSquared;
        hit.hasHit = true;
        return true;
    }
    return false;
}

bool Collider::CheckCircleSquare(float _rad1, vec3 _pos1, vec3 _dimensions, vec3 _pos2, Hit& hit) const
{
    hit.hasHit = false;
    vec3 halfExtents = _dimensions * 0.5f;

    vec3 squareMin = _pos2 - halfExtents;
    vec3 squareMax = _pos2 + halfExtents;

    vec3 closestPoint;
    closestPoint.x = std::max(squareMin.x, std::min(_pos1.x, squareMax.x));
    closestPoint.y = std::max(squareMin.y, std::min(_pos1.y, squareMax.y));
    closestPoint.z = std::max(squareMin.z, std::min(_pos1.z, squareMax.z));

    vec3 delta = closestPoint - _pos1;
    float distanceSquared = dot(delta, delta);

    bool isColliding = distanceSquared <= (_rad1 * _rad1);
    if (isColliding)
    {
        float distance = sqrt(distanceSquared);
        vec3 hitNormal = (distance > 0.0001f) ? delta / distance : vec3(1.0f, 0.0f, 0.0f);
        hit.point = _pos1 + hitNormal * _rad1;
        hit.normal = hitNormal;
        hit.distSQ = distanceSquared;
        hit.hasHit = true;
    }

    return isColliding;
}

bool Collider::CheckSquareSquare(vec3 _dimensions1, vec3 _pos1, vec3 _dimensions2, vec3 _pos2, Hit& hit) const
{
    hit.hasHit = false;
    vec3 halfExtents1 = _dimensions1 * 0.5f;
    vec3 halfExtents2 = _dimensions2 * 0.5f;

    vec3 box1Min = _pos1 - halfExtents1;
    vec3 box1Max = _pos1 + halfExtents1;
    vec3 box2Min = _pos2 - halfExtents2;
    vec3 box2Max = _pos2 + halfExtents2;

    // Check for separation on each axis
    if (box1Max.x < box2Min.x || box1Min.x > box2Max.x) return false;
    if (box1Max.y < box2Min.y || box1Min.y > box2Max.y) return false;
    if (box1Max.z < box2Min.z || box1Min.z > box2Max.z) return false;

    // Calculate overlap on each axis
    vec3 overlap;
    overlap.x = std::min(box1Max.x, box2Max.x) - std::max(box1Min.x, box2Min.x);
    overlap.y = std::min(box1Max.y, box2Max.y) - std::max(box1Min.y, box2Min.y);
    overlap.z = std::min(box1Max.z, box2Max.z) - std::max(box1Min.z, box2Min.z);

    // Find the axis with minimum penetration (shallowest collision)
    float minOverlap = overlap.x;
    vec3 normal(1.0f, 0.0f, 0.0f); // Default to x-axis normal

    if (overlap.y < minOverlap) {
        minOverlap = overlap.y;
        normal = vec3(0.0f, 1.0f, 0.0f);
    }
    if (overlap.z < minOverlap) {
        minOverlap = overlap.z;
        normal = vec3(0.0f, 0.0f, 1.0f);
    }

    // Determine normal direction (from box2 to box1)
    vec3 direction = _pos1 - _pos2;
    if (dot(direction, normal) < 0) {
        normal = -normal;
    }

    // Calculate hit point (center of collision area)
    vec3 collisionCenter;
    collisionCenter.x = std::max(box1Min.x, box2Min.x) + std::min(box1Max.x, box2Max.x);
    collisionCenter.y = std::max(box1Min.y, box2Min.y) + std::min(box1Max.y, box2Max.y);
    collisionCenter.z = std::max(box1Min.z, box2Min.z) + std::min(box1Max.z, box2Max.z);
    collisionCenter *= 0.5f;

    // Fill hit information
    hit.point = collisionCenter;
    hit.normal = normal;
    hit.distSQ = minOverlap * minOverlap;
    hit.hasHit = true;

    return true;
}

void Collider::UpdateMovement(vec3 _normal)
{
    if (profile.type == ColliderType::Dynamic)
    {
        if (!movement)
        {
            movement = parent->GetComponent<Movement>();
            if (!movement)
            {
                Logger::Log(LogLevel::Error, "Dynamic Object doesn't have movement component");
            }
        }
        movement->impactNormal = _normal;
    }
}



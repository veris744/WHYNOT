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

bool Collider::CheckCapsuleCircle(float _rad1, float _height1, vec3 _pos1, float _rad2, vec3 _pos2, Hit& hit) const
{
    hit.hasHit = false;
    // Capsule segment: from bottom to top
    float halfHeight = (_height1 * 0.5f) - _rad1;
    vec3 p1 = _pos1 + vec3(0, -halfHeight, 0); // bottom of capsule segment
    vec3 p2 = _pos1 + vec3(0, +halfHeight, 0); // top of capsule segment

    // Closest point from sphere center (_pos2) to capsule segment
    vec3 segDir = p2 - p1;
    float t = dot(_pos2 - p1, segDir) / dot(segDir, segDir);
    t =  std::clamp(t, 0.0f, 1.0f);
    vec3 closest = p1 + t * segDir;

    vec3 diff = _pos2 - closest;
    float distSq = dot(diff, diff);
    float radiusSum = _rad1 + _rad2;

    if (distSq <= radiusSum * radiusSum) {
        hit.hasHit = true;
        hit.distSQ = distSq;
        hit.point = closest;
        hit.normal = normalize(diff);
        return true;
    }

    return false;
}

bool Collider::CheckCapsuleSquare(float _rad1, float _height1, vec3 _pos1, vec3 _dimensions2, vec3 _pos2,
    Hit& hit) const
{
    hit.hasHit = false;
    float halfHeight = (_height1 * 0.5f) - _rad1;
    vec3 capStart = _pos1 + vec3(0, -halfHeight, 0);
    vec3 capEnd = _pos1 + vec3(0, +halfHeight, 0);

    // Treat the box as an AABB centered at _pos2
    vec3 boxMin = _pos2 - _dimensions2 * 0.5f;
    vec3 boxMax = _pos2 + _dimensions2 * 0.5f;

    // Sample closest point on capsule segment to the box
    vec3 closestOnSeg;
    float minDistSq = FLT_MAX;

    const int steps = 10;
    for (int i = 0; i <= steps; ++i) {
        float t = float(i) / float(steps);
        vec3 pointOnSeg = mix(capStart, capEnd, t);
        vec3 clamped = clamp(pointOnSeg, boxMin, boxMax);
        float dSq = dot(clamped - pointOnSeg, clamped - pointOnSeg);
        if (dSq < minDistSq) {
            minDistSq = dSq;
            closestOnSeg = pointOnSeg;
            hit.point = clamped;
        }
    }

    if (minDistSq <= _rad1 * _rad1) {
        hit.hasHit = true;
        hit.distSQ = minDistSq;
        hit.normal = normalize(hit.point - closestOnSeg);
        return true;
    }

    return false;
}

bool Collider::CheckCapsuleCapsule(float _rad1, float _height1, vec3 _pos1, float _rad2, float _height2, vec3 _pos2,
    Hit& hit) const
{
    hit.hasHit = false;
    float h1 = (_height1 * 0.5f) - _rad1;
    vec3 A0 = _pos1 + vec3(0, -h1, 0);
    vec3 A1 = _pos1 + vec3(0, +h1, 0);

    float h2 = (_height2 * 0.5f) - _rad2;
    vec3 B0 = _pos2 + vec3(0, -h2, 0);
    vec3 B1 = _pos2 + vec3(0, +h2, 0);

    // Compute closest points between segments A and B
    vec3 d1 = A1 - A0;
    vec3 d2 = B1 - B0;
    vec3 r = A0 - B0;

    float a = dot(d1, d1);
    float e = dot(d2, d2);
    float f = dot(d2, r);

    float s = 0.0f, t = 0.0f;

    float c = dot(d1, r);
    float b = dot(d1, d2);
    float denom = a * e - b * b;

    if (denom != 0.0f)
        s = std::clamp((b * f - c * e) / denom, 0.0f, 1.0f);

    t = (b * s + f) / e;
    t = std::clamp(t, 0.0f, 1.0f);

    s = (b * t - c) / a;
    s = std::clamp(s, 0.0f, 1.0f);

    vec3 pt1 = A0 + d1 * s;
    vec3 pt2 = B0 + d2 * t;
    vec3 diff = pt2 - pt1;
    float distSq = dot(diff, diff);
    float radiusSum = _rad1 + _rad2;

    if (distSq <= radiusSum * radiusSum) {
        hit.hasHit = true;
        hit.distSQ = distSq;
        hit.point = pt1 + normalize(diff) * _rad1; // Approximate collision point
        hit.normal = normalize(diff);
        return true;
    }

    return false;
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



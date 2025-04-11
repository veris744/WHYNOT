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

bool Collider::CheckCircleCircle(float _radCircle, vec3 _posCircle, float _radCircle2, vec3 _posCircle2, Hit& hit) const
{
    hit.hasHit = false;
    vec3 delta = _posCircle2 - _posCircle;
    float distanceSquared = dot(delta, delta);
    float combinedRadius = _radCircle + _radCircle2;
    float combinedRadiusSquared = combinedRadius * combinedRadius;

    if (distanceSquared <= combinedRadiusSquared)
    {
        float distance = sqrt(distanceSquared);
        hit.normal = (distance > 0.0001f) ? delta / distance : vec3(1.0f, 0.0f, 0.0f);
        float penetration = combinedRadius - distance;
        hit.point = _posCircle + hit.normal * (_radCircle - penetration * 0.5f);
        hit.distSQ = distanceSquared;
        hit.hasHit = true;
        return true;
    }
    return false;
}

bool Collider::CheckCircleSquare(float _radCircle, vec3 _posCircle, vec3 _dimensionsSquare, vec3 _posSquare, Hit& hit) const
{
    hit.hasHit = false;
    vec3 halfExtents = _dimensionsSquare * 0.5f;

    vec3 squareMin = _posSquare - halfExtents;
    vec3 squareMax = _posSquare + halfExtents;

    vec3 closestPoint;
    closestPoint.x = std::max(squareMin.x, std::min(_posCircle.x, squareMax.x));
    closestPoint.y = std::max(squareMin.y, std::min(_posCircle.y, squareMax.y));
    closestPoint.z = std::max(squareMin.z, std::min(_posCircle.z, squareMax.z));

    vec3 delta = closestPoint - _posCircle;
    float distanceSquared = dot(delta, delta);

    bool isColliding = distanceSquared <= (_radCircle * _radCircle);
    if (isColliding)
    {
        float distance = sqrt(distanceSquared);
        vec3 hitNormal = (distance > 0.0001f) ? delta / distance : vec3(1.0f, 0.0f, 0.0f);
        hit.point = _posCircle + hitNormal * _radCircle;
        hit.normal = hitNormal;
        hit.distSQ = distanceSquared;
        hit.hasHit = true;
    }

    return isColliding;
}

bool Collider::CheckSquareSquare(vec3 _dimensionsSquare, vec3 _posSquare, vec3 _dimensionsSquare2, vec3 _posSquare2, Hit& hit) const
{
    hit.hasHit = false;
    vec3 halfExtents1 = _dimensionsSquare * 0.5f;
    vec3 halfExtents2 = _dimensionsSquare2 * 0.5f;

    vec3 box1Min = _posSquare - halfExtents1;
    vec3 box1Max = _posSquare + halfExtents1;
    vec3 box2Min = _posSquare2 - halfExtents2;
    vec3 box2Max = _posSquare2 + halfExtents2;

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
    vec3 direction = _posSquare - _posSquare2;
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

bool Collider::CheckCapsuleCircle(float _radCapsule, float _heightCapsule, vec3 _posCapsule, float _radCircle, vec3 _posCircle, Hit& hit) const
{
    hit.hasHit = false;
    // Capsule segment: from bottom to top
    float halfHeight = (_heightCapsule * 0.5f) - _radCapsule;
    vec3 p1 = _posCapsule + vec3(0, -halfHeight, 0); // bottom of capsule segment
    vec3 p2 = _posCapsule + vec3(0, +halfHeight, 0); // top of capsule segment

    // Closest point from sphere center (_pos2) to capsule segment
    vec3 segDir = p2 - p1;
    float t = dot(_posCircle - p1, segDir) / dot(segDir, segDir);
    t =  std::clamp(t, 0.0f, 1.0f);
    vec3 closest = p1 + t * segDir;

    vec3 diff = _posCircle - closest;
    float distSq = dot(diff, diff);
    float radiusSum = _radCapsule + _radCircle;

    if (distSq <= radiusSum * radiusSum) {
        hit.hasHit = true;
        hit.distSQ = distSq;
        hit.point = closest;
        hit.normal = normalize(diff);
        return true;
    }

    return false;
}

bool Collider::CheckCapsuleSquare(float _radCapsule, float _heightCapsule, vec3 _posCapsule, vec3 _dimensionsSquare, vec3 _posSquare, Hit& hit) const
{
    hit.hasHit = false;
    float halfHeight = (_heightCapsule * 0.5f) - _radCapsule;
    vec3 capStart = _posCapsule + vec3(0, -halfHeight, 0);
    vec3 capEnd = _posCapsule + vec3(0, +halfHeight, 0);

    // Treat the box as an AABB centered at _pos2
    vec3 boxMin = _posSquare - _dimensionsSquare * 0.5f;
    vec3 boxMax = _posSquare + _dimensionsSquare * 0.5f;

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

    if (minDistSq <= _radCapsule * _radCapsule) {
        hit.hasHit = true;
        hit.distSQ = minDistSq;
        hit.normal = normalize(hit.point - closestOnSeg);
        return true;
    }

    return false;
}

bool Collider::CheckCapsuleCapsule(float _radCapsule, float _heightCapsule, vec3 _posCapsule, float _radCapsule2, float _heightCapsule2, vec3 _posCapsule2, Hit& hit) const
{
    hit.hasHit = false;
    float h1 = (_heightCapsule * 0.5f) - _radCapsule;
    vec3 A0 = _posCapsule + vec3(0, -h1, 0);
    vec3 A1 = _posCapsule + vec3(0, +h1, 0);

    float h2 = (_heightCapsule2 * 0.5f) - _radCapsule2;
    vec3 B0 = _posCapsule2 + vec3(0, -h2, 0);
    vec3 B1 = _posCapsule2 + vec3(0, +h2, 0);

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
    float radiusSum = _radCapsule + _radCapsule2;

    if (distSq <= radiusSum * radiusSum) {
        hit.hasHit = true;
        hit.distSQ = distSq;
        hit.point = pt1 + normalize(diff) * _radCapsule; // Approximate collision point
        hit.normal = normalize(diff);
        return true;
    }

    return false;
}

bool Collider::CheckPlaneCircle(vec2 _dimensionsPlane, vec3 _posPlane, float _radCircle, vec3 _posCircle, Hit& hit) const
{
    vec3 planeNormal(0, 1, 0);
    vec3 planeRight(1, 0, 0);
    vec3 planeForward(0, 0, 1);

    // Distance from sphere to plane
    float dist = dot(_posCircle - _posPlane, planeNormal);

    if (fabs(dist) > _radCircle) return false;

    // Project sphere center onto plane
    vec3 projected = _posCircle - planeNormal * dist;

    // Check if within plane bounds
    vec3 localPos = projected - _posPlane;
    float rightDist = dot(localPos, planeRight);
    float forwardDist = dot(localPos, planeForward);

    float halfWidth = _dimensionsPlane.x * 0.5f;
    float halfLength = _dimensionsPlane.y * 0.5f;

    if (fabs(rightDist) > halfWidth || fabs(forwardDist) > halfLength) {
        // Find closest point on plane edges
        vec3 clampedPoint = _posPlane;
        clampedPoint += planeRight * glm::clamp(rightDist, -halfWidth, halfWidth);
        clampedPoint += planeForward * glm::clamp(forwardDist, -halfLength, halfLength);

        float edgeDist = length(_posCircle - clampedPoint);
        if (edgeDist > _radCircle) return false;

        hit.point = clampedPoint;
    } else {
        hit.point = projected;
    }

    hit.hasHit = true;
    hit.distSQ = (fabs(dist) - _radCircle) * (fabs(dist) - _radCircle);
    hit.normal = (dist > 0) ? planeNormal : -planeNormal;
    return true;
}

bool Collider::CheckPlaneSquare(vec2 _dimensionsPlane, vec3 _posPlane, vec3 _dimensionsSquare, vec3 _posSquare, Hit& hit) const
{
    vec3 planeNormal(0, 1, 0);
    vec3 planeRight(1, 0, 0);
    vec3 planeForward(0, 0, 1);

    // Check if square is parallel to plane
    if (fabs(dot(planeNormal, vec3(0,1,0)) < 0.999f)) return false;

    // Square bounds
    vec3 squareHalf = _dimensionsSquare * 0.5f;
    vec3 squareMin = _posSquare - squareHalf;
    vec3 squareMax = _posSquare + squareHalf;

    // Plane bounds
    float halfWidth = _dimensionsPlane.x * 0.5f;
    float halfLength = _dimensionsPlane.y * 0.5f;

    // Find overlap area
    vec3 planeMin = _posPlane - planeRight * halfWidth - planeForward * halfLength;
    vec3 planeMax = _posPlane + planeRight * halfWidth + planeForward * halfLength;

    if (squareMax.x < planeMin.x || squareMin.x > planeMax.x ||
        squareMax.z < planeMin.z || squareMin.z > planeMax.z)
        return false;

    // Calculate collision data
    hit.hasHit = true;
    hit.point = (min(squareMax, planeMax) + max(squareMin, planeMin)) * 0.5f;
    hit.normal = planeNormal;
    hit.distSQ = 0; // They're touching

    return true;
}

bool Collider::CheckPlaneCapsule(vec2 _dimensionsPlane, vec3 _posPlane, float _radCapsule, float _heightCapsule, vec3 _posCapsule, Hit& hit) const
{
    hit.hasHit = false;

    // Calculate capsule endpoints (Y-axis aligned)
    float halfHeight = (_heightCapsule * 0.5f) - _radCapsule;
    vec3 capStart = _posCapsule + vec3(0, -halfHeight, 0);
    vec3 capEnd = _posCapsule + vec3(0, halfHeight, 0);

    // Plane parameters (Y-up)
    vec3 planeNormal(0, 1, 0);
    float planeY = _posPlane.y;

    // Sample points along capsule segment
    const int steps = 10;
    float minDistSq = FLT_MAX;
    vec3 closestOnSeg;
    vec3 closestPoint;

    for (int i = 0; i <= steps; ++i) {
        float t = float(i) / float(steps);
        vec3 pointOnSeg = mix(capStart, capEnd, t);

        // Distance to plane
        float dist = pointOnSeg.y - planeY;

        // Project point onto plane
        vec3 projected = pointOnSeg;
        projected.y = planeY;

        // Check if within plane bounds (if finite)
        if (_dimensionsPlane.x > 0 && _dimensionsPlane.y > 0) {
            vec3 localPos = projected - _posPlane;
            float rightDist = localPos.x;
            float forwardDist = localPos.z;
            float halfWidth = _dimensionsPlane.x * 0.5f;
            float halfLength = _dimensionsPlane.y * 0.5f;

            if (fabs(rightDist) > halfWidth || fabs(forwardDist) > halfLength) {
                // Find closest point on plane perimeter
                vec3 edgePoint = _posPlane;
                edgePoint.x += glm::clamp(rightDist, -halfWidth, halfWidth);
                edgePoint.z += glm::clamp(forwardDist, -halfLength, halfLength);
                edgePoint.y = planeY;

                float edgeDistSq = dot(pointOnSeg - edgePoint, pointOnSeg - edgePoint);
                if (edgeDistSq < minDistSq) {
                    minDistSq = edgeDistSq;
                    closestOnSeg = pointOnSeg;
                    hit.point = edgePoint;
                }
                continue;
            }
        }

        // For infinite plane or points within bounds
        float distSq = dist * dist;
        if (distSq < minDistSq) {
            minDistSq = distSq;
            closestOnSeg = pointOnSeg;
            hit.point = projected;
        }
    }

    // Check if within capsule radius
    if (minDistSq <= _radCapsule * _radCapsule) {
        hit.hasHit = true;
        hit.distSQ = minDistSq;

        // Calculate normal (from plane to capsule)
        if (minDistSq > 0.0001f) {
            hit.normal = normalize(hit.point - closestOnSeg);
        } else {
            hit.normal = planeNormal;
        }

        // Adjust hit point to be on capsule surface
        hit.point = closestOnSeg + hit.normal * _radCapsule;
        return true;
    }

    return false;
}

bool Collider::CheckPlanePlane(vec2 _dimensionsPlane, vec3 _posPlane, vec2 _dimensionsPlane2, vec3 _posPlane2, Hit& hit) const
{
    vec3 normal1(0, 1, 0);
    vec3 normal2(0, 1, 0);
    float d1 = _posPlane.z;
    float d2 = _posPlane2.z;

    // Planes are parallel
    if (abs(dot(normal1, normal2)) > 0.999f) {
        // Check if coincident
        if (abs(d1 - d2) < 0.001f) {
            hit.hasHit = true;
            hit.distSQ = 0;
            hit.point = vec3(0); // No single intersection point
            hit.normal = normal1;
            return true;
        }
        return false;
    }

    // Planes intersect (infinite line)
    hit.hasHit = true;
    hit.distSQ = 0;
    hit.point = vec3(0); // No single intersection point
    hit.normal = normalize(cross(normal1, normal2));
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
        movement->collisionNormals.push_back(_normal);
    }
}



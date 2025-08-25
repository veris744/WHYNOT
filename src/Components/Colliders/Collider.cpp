#include "Collider.h"

#include <Utils/Debugger.h>

#include "Entities/Entity.h"

glm::vec3 Collider::GetWorldPosition()
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

    if (profile.mode == ColliderMode::Physics || profile.mode == ColliderMode::All)
    {
        CollisionDelegate.Bind(&Entity::OnCollision, parent);
    }
}

bool Collider::CheckCircleCircle(float _radCircle, glm::vec3 _posCircle, float _radCircle2, glm::vec3 _posCircle2, Hit& hit) const
{
    hit.hasHit = false;
    glm::vec3 delta = _posCircle2 - _posCircle;
    float distanceSquared = dot(delta, delta);
    float combinedRadius = _radCircle + _radCircle2;
    float combinedRadiusSquared = combinedRadius * combinedRadius;

    if (distanceSquared <= combinedRadiusSquared)
    {
        float distance = sqrt(distanceSquared);
        hit.normal = (distance > 0.0001f) ? delta / distance : glm::vec3(1.0f, 0.0f, 0.0f);
        float penetration = combinedRadius - distance;
        hit.point = _posCircle + hit.normal * (_radCircle - penetration * 0.5f);
        hit.distSQ = distanceSquared;
        hit.hasHit = true;
        return true;
    }
    return false;
}

bool Collider::CheckCircleSquare(float _radCircle, glm::vec3 _posCircle, glm::vec3 _dimensionsSquare, glm::vec3 _posSquare, Hit& hit) const
{
    hit.hasHit = false;
    glm::vec3 halfExtents = _dimensionsSquare * 0.5f;

    glm::vec3 squareMin = _posSquare - halfExtents;
    glm::vec3 squareMax = _posSquare + halfExtents;

    glm::vec3 closestPoint;
    closestPoint.x = std::max(squareMin.x, std::min(_posCircle.x, squareMax.x));
    closestPoint.y = std::max(squareMin.y, std::min(_posCircle.y, squareMax.y));
    closestPoint.z = std::max(squareMin.z, std::min(_posCircle.z, squareMax.z));

    glm::vec3 delta = closestPoint - _posCircle;
    float distanceSquared = dot(delta, delta);

    bool isColliding = distanceSquared <= (_radCircle * _radCircle);
    if (isColliding)
    {
        float distance = sqrt(distanceSquared);
        glm::vec3 hitNormal = (distance > 0.0001f) ? delta / distance : glm::vec3(1.0f, 0.0f, 0.0f);
        hit.point = _posCircle + hitNormal * _radCircle;
        hit.normal = hitNormal;
        hit.distSQ = distanceSquared;
        hit.hasHit = true;
    }

    return isColliding;
}

bool Collider::CheckSquareSquare(glm::vec3 _dimensionsSquare, glm::vec3 _posSquare, glm::vec3 _dimensionsSquare2, glm::vec3 _posSquare2, Hit& hit) const
{
    hit.hasHit = false;
    glm::vec3 halfExtents1 = _dimensionsSquare * 0.5f;
    glm::vec3 halfExtents2 = _dimensionsSquare2 * 0.5f;

    glm::vec3 box1Min = _posSquare - halfExtents1;
    glm::vec3 box1Max = _posSquare + halfExtents1;
    glm::vec3 box2Min = _posSquare2 - halfExtents2;
    glm::vec3 box2Max = _posSquare2 + halfExtents2;

    // Check for separation on each axis
    if (box1Max.x < box2Min.x || box1Min.x > box2Max.x) return false;
    if (box1Max.y < box2Min.y || box1Min.y > box2Max.y) return false;
    if (box1Max.z < box2Min.z || box1Min.z > box2Max.z) return false;

    // Calculate overlap on each axis
    glm::vec3 overlap;
    overlap.x = std::min(box1Max.x, box2Max.x) - std::max(box1Min.x, box2Min.x);
    overlap.y = std::min(box1Max.y, box2Max.y) - std::max(box1Min.y, box2Min.y);
    overlap.z = std::min(box1Max.z, box2Max.z) - std::max(box1Min.z, box2Min.z);

    // Find the axis with minimum penetration (shallowest collision)
    float minOverlap = overlap.x;
    glm::vec3 normal(1.0f, 0.0f, 0.0f); // Default to x-axis normal

    if (overlap.y < minOverlap) {
        minOverlap = overlap.y;
        normal = glm::vec3(0.0f, 1.0f, 0.0f);
    }
    if (overlap.z < minOverlap) {
        minOverlap = overlap.z;
        normal = glm::vec3(0.0f, 0.0f, 1.0f);
    }

    // Determine normal direction (from box2 to box1)
    glm::vec3 direction = _posSquare - _posSquare2;
    if (dot(direction, normal) < 0) {
        normal = -normal;
    }

    // Calculate hit point (center of collision area)
    glm::vec3 collisionCenter;
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

bool Collider::CheckCapsuleCircle(float _radCapsule, float _heightCapsule, glm::vec3 _posCapsule, float _radCircle, glm::vec3 _posCircle, Hit& hit) const
{
    hit.hasHit = false;
    // Capsule segment: from bottom to top
    float halfHeight = (_heightCapsule * 0.5f) - _radCapsule;
    glm::vec3 p1 = _posCapsule + glm::vec3(0, -halfHeight, 0); // bottom of capsule segment
    glm::vec3 p2 = _posCapsule + glm::vec3(0, +halfHeight, 0); // top of capsule segment

    // Closest point from sphere center (_pos2) to capsule segment
    glm::vec3 segDir = p2 - p1;
    float t = dot(_posCircle - p1, segDir) / dot(segDir, segDir);
    t =  std::clamp(t, 0.0f, 1.0f);
    glm::vec3 closest = p1 + t * segDir;

    glm::vec3 diff = _posCircle - closest;
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

bool Collider::CheckCapsuleSquare(float _radCapsule, float _heightCapsule, glm::vec3 _posCapsule, glm::vec3 _dimensionsSquare, glm::vec3 _posSquare, Hit& hit) const
{
    hit.hasHit = false;
    float halfHeight = (_heightCapsule * 0.5f) - _radCapsule;
    glm::vec3 capStart = _posCapsule + glm::vec3(0, -halfHeight, 0);
    glm::vec3 capEnd = _posCapsule + glm::vec3(0, +halfHeight, 0);

    // Treat the box as an AABB centered at _pos2
    glm::vec3 boxMin = _posSquare - _dimensionsSquare * 0.5f;
    glm::vec3 boxMax = _posSquare + _dimensionsSquare * 0.5f;

    // Sample closest point on capsule segment to the box
    glm::vec3 closestOnSeg;
    float minDistSq = FLT_MAX;

    const int steps = 10;
    for (int i = 0; i <= steps; ++i) {
        float t = float(i) / float(steps);
        glm::vec3 pointOnSeg = mix(capStart, capEnd, t);
        glm::vec3 clamped = clamp(pointOnSeg, boxMin, boxMax);
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

bool Collider::CheckCapsuleCapsule(float _radCapsule, float _heightCapsule, glm::vec3 _posCapsule, float _radCapsule2, float _heightCapsule2, glm::vec3 _posCapsule2, Hit& hit) const
{
    hit.hasHit = false;
    float h1 = (_heightCapsule * 0.5f) - _radCapsule;
    glm::vec3 A0 = _posCapsule + glm::vec3(0, -h1, 0);
    glm::vec3 A1 = _posCapsule + glm::vec3(0, +h1, 0);

    float h2 = (_heightCapsule2 * 0.5f) - _radCapsule2;
    glm::vec3 B0 = _posCapsule2 + glm::vec3(0, -h2, 0);
    glm::vec3 B1 = _posCapsule2 + glm::vec3(0, +h2, 0);

    // Compute closest points between segments A and B
    glm::vec3 d1 = A1 - A0;
    glm::vec3 d2 = B1 - B0;
    glm::vec3 r = A0 - B0;

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

    glm::vec3 pt1 = A0 + d1 * s;
    glm::vec3 pt2 = B0 + d2 * t;
    glm::vec3 diff = pt2 - pt1;
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

bool Collider::CheckPlaneCircle(glm::vec2 _dimensionsPlane, glm::vec3 _posPlane, glm::vec3 _normalPlane, float _radCircle, glm::vec3 _posCircle, Hit& hit) const
{
    // Normalize the plane normal to ensure correct calculations
    glm::vec3 planeNormal = normalize(_normalPlane);

    // Calculate right and forward vectors relative to the plane's orientation
    glm::vec3 planeRight;
    if (fabs(planeNormal.y) > 0.999f) {
        // If plane is mostly aligned with world up/down, use world right
        planeRight = glm::vec3(1, 0, 0);
    } else {
        // Otherwise, calculate right vector using cross product with world up
        planeRight = normalize(cross(planeNormal, glm::vec3(0, 1, 0)));
    }

    glm::vec3 planeForward = normalize(cross(planeRight, planeNormal));

    // Distance from sphere to plane (signed distance)
    float dist = dot(_posCircle - _posPlane, planeNormal);

    if (fabs(dist) > _radCircle) return false;

    // Project sphere center onto plane
    glm::vec3 projected = _posCircle - planeNormal * dist;

    // Check if within plane bounds
    glm::vec3 localPos = projected - _posPlane;
    float rightDist = dot(localPos, planeRight);
    float forwardDist = dot(localPos, planeForward);

    float halfWidth = _dimensionsPlane.x * 0.5f;
    float halfLength = _dimensionsPlane.y * 0.5f;

    if (fabs(rightDist) > halfWidth || fabs(forwardDist) > halfLength) {
        // Find closest point on plane edges
        glm::vec3 clampedPoint = _posPlane;
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

bool Collider::CheckPlaneSquare(glm::vec2 _dimensionsPlane, glm::vec3 _posPlane, glm::vec3 _normalPlane, glm::vec3 _dimensionsSquare, glm::vec3 _posSquare, Hit& hit) const
{
    glm::vec3 planeNormal(0, 1, 0);
    glm::vec3 planeRight(1, 0, 0);
    glm::vec3 planeForward(0, 0, 1);

    // Check if square is parallel to plane
    if (fabs(dot(planeNormal, glm::vec3(0,1,0)) < 0.999f)) return false;

    // Square bounds
    glm::vec3 squareHalf = _dimensionsSquare * 0.5f;
    glm::vec3 squareMin = _posSquare - squareHalf;
    glm::vec3 squareMax = _posSquare + squareHalf;

    // Plane bounds
    float halfWidth = _dimensionsPlane.x * 0.5f;
    float halfLength = _dimensionsPlane.y * 0.5f;

    // Find overlap area
    glm::vec3 planeMin = _posPlane - planeRight * halfWidth - planeForward * halfLength;
    glm::vec3 planeMax = _posPlane + planeRight * halfWidth + planeForward * halfLength;

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

bool Collider::CheckPlaneCapsule(glm::vec2 _dimensionsPlane, glm::vec3 _posPlane, glm::vec3 _normalPlane, float _radCapsule, float _heightCapsule, glm::vec3 _posCapsule, Hit& hit) const
{
    hit.hasHit = false;

    // Calculate capsule endpoints (Y-axis aligned)
    float halfHeight = (_heightCapsule * 0.5f) - _radCapsule;
    glm::vec3 capStart = _posCapsule + glm::vec3(0, -halfHeight, 0);
    glm::vec3 capEnd = _posCapsule + glm::vec3(0, halfHeight, 0);

    // Plane parameters (Y-up)
    glm::vec3 planeNormal(0, 1, 0);
    float planeY = _posPlane.y;

    // Sample points along capsule segment
    const int steps = 10;
    float minDistSq = FLT_MAX;
    glm::vec3 closestOnSeg;
    glm::vec3 closestPoint;

    for (int i = 0; i <= steps; ++i) {
        float t = float(i) / float(steps);
        glm::vec3 pointOnSeg = mix(capStart, capEnd, t);

        // Distance to plane
        float dist = pointOnSeg.y - planeY;

        // Project point onto plane
        glm::vec3 projected = pointOnSeg;
        projected.y = planeY;

        // Check if within plane bounds (if finite)
        if (_dimensionsPlane.x > 0 && _dimensionsPlane.y > 0) {
            glm::vec3 localPos = projected - _posPlane;
            float rightDist = localPos.x;
            float forwardDist = localPos.z;
            float halfWidth = _dimensionsPlane.x * 0.5f;
            float halfLength = _dimensionsPlane.y * 0.5f;

            if (fabs(rightDist) > halfWidth || fabs(forwardDist) > halfLength) {
                // Find closest point on plane perimeter
                glm::vec3 edgePoint = _posPlane;
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

bool Collider::CheckSlopeCircle(glm::vec3 _dimensionsSlope, glm::vec3 _posSlope, float _radCircle, glm::vec3 _posCircle, Hit& hit) const
{
    hit.hasHit = false;
    glm::vec3 halfExtents = _dimensionsSlope * 0.5f;

    // Transform to local slope space
    glm::vec3 localCircle = _posCircle - _posSlope;

    // AABB check first (early out)
    glm::vec3 aabbMin = -halfExtents;
    glm::vec3 aabbMax = halfExtents;
    glm::vec3 closestAABB;
    closestAABB.x = std::max(aabbMin.x, std::min(localCircle.x, aabbMax.x));
    closestAABB.y = std::max(aabbMin.y, std::min(localCircle.y, aabbMax.y));
    closestAABB.z = std::max(aabbMin.z, std::min(localCircle.z, aabbMax.z));

    float distSqAABB = dot(closestAABB - localCircle, closestAABB - localCircle);
    if (distSqAABB > (_radCircle * _radCircle)) return false;

    // Define slope plane (diagonal from bottom-front to top-back)
    glm::vec3 slopeNormal = normalize(glm::vec3(0.0f, halfExtents.z, halfExtents.y));
    glm::vec3 slopePoint(0.0f, -halfExtents.y, halfExtents.z); // Point on slope plane

    float distToPlane = dot(localCircle - slopePoint, slopeNormal);
    bool aboveSlope = distToPlane > 0;

    // 1. Check collision with slope plane
    if (abs(distToPlane) <= _radCircle) {
        glm::vec3 projectedPoint = localCircle - slopeNormal * distToPlane;

        // Check if projected point is within slope bounds
        if (projectedPoint.x >= aabbMin.x && projectedPoint.x <= aabbMax.x &&
            projectedPoint.y >= aabbMin.y && projectedPoint.y <= aabbMax.y &&
            projectedPoint.z >= aabbMin.z && projectedPoint.z <= aabbMax.z)
        {
            hit.hasHit = true;
            hit.point = _posSlope + projectedPoint;
            hit.normal = (distToPlane < 0) ? -slopeNormal : slopeNormal;
            hit.distSQ = distToPlane * distToPlane;
            return true;
        }
    }

    // 2. Check bottom face (only if below slope)
    if (!aboveSlope) {
        float distToBottom = localCircle.y - aabbMin.y;
        if (abs(distToBottom) <= _radCircle) {
            glm::vec3 bottomPoint = localCircle;
            bottomPoint.y = aabbMin.y;

            if (bottomPoint.x >= aabbMin.x && bottomPoint.x <= aabbMax.x &&
                bottomPoint.z >= aabbMin.z && bottomPoint.z <= aabbMax.z)
            {
                hit.hasHit = true;
                hit.point = _posSlope + bottomPoint;
                hit.normal = glm::vec3(0.0f, -1.0f, 0.0f);
                hit.distSQ = distToBottom * distToBottom;
                return true;
            }
        }
    }

    // 3. Check back face (z = -halfExtents.z)
    float distToBack = localCircle.z - aabbMin.z;
    if (abs(distToBack) <= _radCircle) {
        glm::vec3 backPoint = localCircle;
        backPoint.z = aabbMin.z;

        if (backPoint.x >= aabbMin.x && backPoint.x <= aabbMax.x &&
            backPoint.y >= aabbMin.y && backPoint.y <= aabbMax.y)
        {
            hit.hasHit = true;
            hit.point = _posSlope + backPoint;
            hit.normal = glm::vec3(0.0f, 0.0f, -1.0f);
            hit.distSQ = distToBack * distToBack;
            return true;
        }
    }

    // 4. Check side faces (x = ±halfExtents.x)
    // Left face (x = -halfExtents.x)
    float distToLeft = localCircle.x - aabbMin.x;
    if (abs(distToLeft) <= _radCircle) {
        glm::vec3 leftPoint = localCircle;
        leftPoint.x = aabbMin.x;

        if (leftPoint.y >= aabbMin.y && leftPoint.y <= aabbMax.y &&
            leftPoint.z >= aabbMin.z && leftPoint.z <= aabbMax.z)
        {
            hit.hasHit = true;
            hit.point = _posSlope + leftPoint;
            hit.normal = glm::vec3(-1.0f, 0.0f, 0.0f);
            hit.distSQ = distToLeft * distToLeft;
            return true;
        }
    }

    // Right face (x = halfExtents.x)
    float distToRight = localCircle.x - aabbMax.x;
    if (abs(distToRight) <= _radCircle) {
        glm::vec3 rightPoint = localCircle;
        rightPoint.x = aabbMax.x;

        if (rightPoint.y >= aabbMin.y && rightPoint.y <= aabbMax.y &&
            rightPoint.z >= aabbMin.z && rightPoint.z <= aabbMax.z)
        {
            hit.hasHit = true;
            hit.point = _posSlope + rightPoint;
            hit.normal = glm::vec3(1.0f, 0.0f, 0.0f);
            hit.distSQ = distToRight * distToRight;
            return true;
        }
    }

    return false;
}

bool Collider::CheckSlopeSquare(glm::vec3 _dimensionsSlope, glm::vec3 _posSlope, glm::vec3 _dimensionsSquare, glm::vec3 _posSquare, Hit& hit) const
{
    hit.hasHit = false;
    glm::vec3 slopeHalf = _dimensionsSlope * 0.5f;
    glm::vec3 squareHalf = _dimensionsSquare * 0.5f;

    // Simple AABB check first
    glm::vec3 slopeMin = _posSlope - slopeHalf;
    glm::vec3 slopeMax = _posSlope + slopeHalf;
    glm::vec3 squareMin = _posSquare - squareHalf;
    glm::vec3 squareMax = _posSquare + squareHalf;

    if (squareMax.x < slopeMin.x || squareMin.x > slopeMax.x) return false;
    if (squareMax.y < slopeMin.y || squareMin.y > slopeMax.y) return false;
    if (squareMax.z < slopeMin.z || squareMin.z > slopeMax.z) return false;

    // Check against slope plane
    glm::vec3 slopeNormal(0.0f, 0.707f, 0.707f);
    glm::vec3 slopePoint = _posSlope + glm::vec3(0.0f, -slopeHalf.y, slopeHalf.z);

    // Get the square vertices
    glm::vec3 squareVertices[8] = {
        _posSquare + glm::vec3(-squareHalf.x, -squareHalf.y, -squareHalf.z),
        _posSquare + glm::vec3( squareHalf.x, -squareHalf.y, -squareHalf.z),
        _posSquare + glm::vec3( squareHalf.x,  squareHalf.y, -squareHalf.z),
        _posSquare + glm::vec3(-squareHalf.x,  squareHalf.y, -squareHalf.z),
        _posSquare + glm::vec3(-squareHalf.x, -squareHalf.y,  squareHalf.z),
        _posSquare + glm::vec3( squareHalf.x, -squareHalf.y,  squareHalf.z),
        _posSquare + glm::vec3( squareHalf.x,  squareHalf.y,  squareHalf.z),
        _posSquare + glm::vec3(-squareHalf.x,  squareHalf.y,  squareHalf.z)
    };

    // Check if any square vertex is below the slope plane
    bool collision = false;
    float maxPenetration = -FLT_MAX;
    glm::vec3 bestNormal;
    glm::vec3 bestPoint;

    for (int i = 0; i < 8; i++) {
        float dist = dot(squareVertices[i] - slopePoint, slopeNormal);
        if (dist < 0) {
            collision = true;
            if (dist > maxPenetration) {
                maxPenetration = dist;
                bestNormal = slopeNormal;
                bestPoint = squareVertices[i] - slopeNormal * dist * 0.5f;
            }
        }
    }

    if (collision) {
        hit.hasHit = true;
        hit.point = bestPoint;
        hit.normal = bestNormal;
        hit.distSQ = maxPenetration * maxPenetration;
        return true;
    }

    // If no vertex is below the plane, check edge intersections
    // (This is simplified - a full implementation would check all edges)
    return false;
}

bool Collider::CheckSlopeCapsule(glm::vec3 _dimensionsSlope, glm::vec3 _posSlope, float _radCapsule, float _heightCapsule, glm::vec3 _posCapsule, Hit& hit) const
{
    hit.hasHit = false;
    glm::vec3 halfExtents = _dimensionsSlope * 0.5f;

    // Transform capsule to local slope space
    glm::vec3 localCapsule = _posCapsule - _posSlope;

    // Capsule line segment endpoints (assuming capsule is aligned with Y-axis)
    glm::vec3 capsuleDir(0.0f, 1.0f, 0.0f); // Direction of the capsule (adjust if needed)
    float halfHeight = _heightCapsule * 0.5f - _radCapsule; // Subtract radius to get inner segment
    if (halfHeight < 0.0f) halfHeight = 0.0f; // Clamp in case radius > height

    glm::vec3 pointA = localCapsule - capsuleDir * halfHeight;
    glm::vec3 pointB = localCapsule + capsuleDir * halfHeight;

    struct SlopeFace
    {
        glm::vec3 normal;
        glm::vec3 point;
        glm::vec3 minBound;
        glm::vec3 maxBound;
    };

    // 1. Top diagonal face
    SlopeFace topFace;
    topFace.normal = normalize(glm::vec3(0.0f, halfExtents.z, halfExtents.y));
    topFace.point = glm::vec3(0.0f, 0.0f, 0.0f);
    topFace.minBound = glm::vec3(-halfExtents.x, -halfExtents.y, -halfExtents.z);
    topFace.maxBound = glm::vec3(halfExtents.x, halfExtents.y, halfExtents.z);

    // 2. Bottom face (Y-)
    SlopeFace bottomFace;
    bottomFace.normal = glm::vec3(0.0f, -1.0f, 0.0f);
    bottomFace.point = glm::vec3(0.0f, -halfExtents.y, 0.0f);
    bottomFace.minBound = glm::vec3(-halfExtents.x, -halfExtents.y, -halfExtents.z);
    bottomFace.maxBound = glm::vec3(halfExtents.x, -halfExtents.y, halfExtents.z);

    // 3. Back face (Z-)
    SlopeFace backFace;
    backFace.normal = glm::vec3(0.0f, 0.0f, -1.0f);
    backFace.point = glm::vec3(0.0f, 0.0f, -halfExtents.z);
    backFace.minBound = glm::vec3(-halfExtents.x, -halfExtents.y, -halfExtents.z);
    backFace.maxBound = glm::vec3(halfExtents.x, halfExtents.y, -halfExtents.z);

    // 5. Side faces (X- and X+)
    SlopeFace leftFace, rightFace;
    leftFace.normal = glm::vec3(-1.0f, 0.0f, 0.0f);
    leftFace.point = glm::vec3(-halfExtents.x, 0.0f, 0.0f);
    leftFace.minBound = glm::vec3(-halfExtents.x, -halfExtents.y, -halfExtents.z);
    leftFace.maxBound = glm::vec3(-halfExtents.x, halfExtents.y, halfExtents.z);

    rightFace.normal = glm::vec3(1.0f, 0.0f, 0.0f);
    rightFace.point = glm::vec3(halfExtents.x, 0.0f, 0.0f);
    rightFace.minBound = glm::vec3(halfExtents.x, -halfExtents.y, -halfExtents.z);
    rightFace.maxBound = glm::vec3(halfExtents.x, halfExtents.y, halfExtents.z);

    // Check all faces
    SlopeFace faces[] = {topFace, bottomFace, backFace, leftFace, rightFace};
    Hit closestHit;
    float minDistSq = FLT_MAX;

    for (const SlopeFace& face : faces) {
        // Find closest point on capsule segment to the plane
        glm::vec3 capsuleToFace = face.point - pointA;
        glm::vec3 segmentDir = pointB - pointA;
        float segmentLength = length(segmentDir);
        if (segmentLength > 0.0f) segmentDir /= segmentLength;

        float t = dot(segmentDir, capsuleToFace);
        t = std::max(0.0f, std::min(t, segmentLength));
        glm::vec3 closestPoint = pointA + segmentDir * t;

        // Check sphere-plane collision
        float distToPlane = dot(closestPoint - face.point, face.normal);

        if (abs(distToPlane) <= _radCapsule) {
            glm::vec3 projectedPoint = closestPoint - face.normal * distToPlane;

            // Check bounds
            if (projectedPoint.x >= face.minBound.x && projectedPoint.x <= face.maxBound.x &&
                projectedPoint.y >= face.minBound.y && projectedPoint.y <= face.maxBound.y &&
                projectedPoint.z >= face.minBound.z && projectedPoint.z <= face.maxBound.z)
            {
                float distSq = distToPlane * distToPlane;
                if (distSq < minDistSq) {
                    minDistSq = distSq;
                    closestHit.hasHit = true;
                    closestHit.point = _posSlope + projectedPoint;
                    closestHit.normal = (distToPlane < 0) ? -face.normal : face.normal;
                    closestHit.distSQ = distSq;
                }
            }
        }
    }

    if (closestHit.hasHit) {
        hit = closestHit;
        return true;
    }

    return false;
}

bool Collider::CheckSlopePlane(glm::vec3 _dimensionsSlope, glm::vec3 _posSlope, glm::vec2 _dimensionsPlane, glm::vec3 _posPlane, Hit& hit) const
{
    hit.hasHit = false;

    // Plane is assumed to be horizontal (normal (0,1,0))
    glm::vec3 planeNormal(0, 1, 0);
    glm::vec3 planeRight(1, 0, 0);
    glm::vec3 planeForward(0, 0, 1);

    // Slope normal
    glm::vec3 slopeNormal(0.0f, 0.707f, 0.707f);

    // If plane is not horizontal or parallel to slope, return false
    if (fabs(dot(planeNormal, slopeNormal)) > 0.001f) return false;

    // Slope bounds
    glm::vec3 slopeHalf = _dimensionsSlope * 0.5f;
    glm::vec3 slopeMin = _posSlope - slopeHalf;
    glm::vec3 slopeMax = _posSlope + slopeHalf;

    // Plane bounds
    float halfWidth = _dimensionsPlane.x * 0.5f;
    float halfLength = _dimensionsPlane.y * 0.5f;

    // Find overlap area in XZ plane
    glm::vec3 planeMin = _posPlane - planeRight * halfWidth - planeForward * halfLength;
    glm::vec3 planeMax = _posPlane + planeRight * halfWidth + planeForward * halfLength;

    if (slopeMax.x < planeMin.x || slopeMin.x > planeMax.x ||
        slopeMax.z < planeMin.z || slopeMin.z > planeMax.z)
        return false;

    // Check if plane is above the highest point of the slope
    float highestSlopeY = slopeMax.y;
    if (_posPlane.y > highestSlopeY) return false;

    // Check if plane is below the lowest point of the slope
    if (_posPlane.y < slopeMin.y) {
        hit.hasHit = true;
        hit.point = (min(slopeMax, glm::vec3(planeMax.x, slopeMax.y, planeMax.z)) +
                    max(slopeMin, glm::vec3(planeMin.x, slopeMin.y, planeMin.z))) * 0.5f;
        hit.normal = planeNormal;
        hit.distSQ = 0;
        return true;
    }

    // Check intersection with slope plane
    float planeDistToSlope = dot(_posPlane - (_posSlope + glm::vec3(0, -slopeHalf.y, slopeHalf.z)), slopeNormal);
    if (fabs(planeDistToSlope) < 0.001f) {
        hit.hasHit = true;
        hit.point = _posPlane;
        hit.normal = slopeNormal;
        hit.distSQ = 0;
        return true;
    }

    return false;
}

bool Collider::CheckSlopeSlope(glm::vec3 _dimensionsSlope, glm::vec3 _posSlope, glm::vec3 _dimensionsSlope2, glm::vec3 _posSlope2,
    Hit& hit) const
{
    return false;
}

void Collider::UpdateMovement(const Hit& hit)
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
        movement->AddCollision(hit);
    }
}



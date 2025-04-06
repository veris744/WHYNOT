#include "CapsuleCollider.h"

#include "BoxCollider.h"
#include "Entities/Entity.h"
#include "Utils/Debugger.h"

vec3 closestPointOnSegment(const vec3& A, const vec3& B, const vec3& point)
{
    vec3 AB = B - A;
    float t = dot(point - A, AB) / dot(AB, AB);
    t = std::clamp(t, 0.0f, 1.0f);
    return A + t * AB;
}

bool CapsuleCollider::Collides(Collider* other, Hit& hit)
{
    if (other->Collides(height, radius, GetWorldPosition(), hit))
    {
        hit.otherEntity = other->parent;
        hit.selfEntity = parent;
        hit.type = World;
        vec3 directionToSelf = GetWorldPosition() - hit.point;
        if (dot(directionToSelf, hit.normal) < 0.0f)
        {
            hit.normal = -hit.normal;
        }
        return true;
    }
    return false;
}

bool CapsuleCollider::Collides(float _rad1, vec3 _pos1, Hit& hit)
{
    return CheckCircleCircle(radius, GetWorldPosition(), _rad1, _pos1, hit);
}

bool CapsuleCollider::Collides(vec3 _dimensions, vec3 _pos1, Hit& hit)
{
    return CheckCapsuleSquare(radius, height, GetWorldPosition(), _dimensions, _pos1, hit);
}

bool CapsuleCollider::Collides(float _height, float _radius, vec3 _pos1, Hit& hit)
{
    return CheckCapsuleCapsule(_radius, _height, _pos1, radius, height, GetWorldPosition(), hit);
}

// Collision from ray staring at origin extending towards infinity
bool CapsuleCollider::RayCollides(vec3 _rayOrigin, vec3 _rayDir, Hit& hit)
{
    float halfHeight = (height * 0.5f) - radius;
    vec3 A = position + vec3(0, -halfHeight, 0); // bottom of capsule
    vec3 B = position + vec3(0, +halfHeight, 0); // top of capsule

    vec3 toCapsule = GetWorldPosition() - _rayOrigin;
    float dotDir = dot(_rayDir, toCapsule);
    if (dotDir < 0)
        return false;

    vec3 d = B - A;
    vec3 m = _rayOrigin - A;
    vec3 n = _rayDir;

    float dd = dot(d, d);
    float nd = dot(n, d);
    float md = dot(m, d);

    float a = dd - nd * nd;
    float b = dd * dot(m, n) - md * nd;
    float c = dd * dot(m, m) - md * md - dd * radius * radius;

    float discriminant = b * b - a * c;

    if (discriminant < 0.0f) return false; // no real roots => no intersection

    float t = (-b - sqrt(discriminant)) / a;
    if (t < 0.0f) return false; // intersection is behind ray

    hit.hasHit = true;
    hit.distSQ = t * t;
    hit.point = _rayOrigin + _rayDir * t;

    // Approximate normal using the closest point on capsule segment
    vec3 closestOnSeg = closestPointOnSegment(A, B, hit.point);
    hit.normal = normalize(hit.point - closestOnSeg);
    return true;
}

bool CapsuleCollider::CheckInBounds(const vec2& xBounds, const vec2& yBounds, const vec2& zBounds, bool triggerElement)
{
    float halfHeight = (height * 0.5f);
    vec3 minPoint = position + vec3(-radius, -halfHeight, -radius);
    vec3 maxPoint = position + vec3(+radius, +halfHeight, +radius);

    bool inside =
        minPoint.x >= xBounds.x && maxPoint.x <= xBounds.y &&
        minPoint.y >= yBounds.x && maxPoint.y <= yBounds.y &&
        minPoint.z >= zBounds.x && maxPoint.z <= zBounds.y;

    if (inside && triggerElement)
    {
        // OnOutOfBoundsDelegate.Execute(normal);
    }

    return inside;
}

bool CapsuleCollider::OverlapsBounds(const vec2& xBounds, const vec2& yBounds, const vec2& zBounds,
    bool triggerDelegate)
{
float halfHeight = (height * 0.5f);
    vec3 minPoint = position + vec3(-radius, -halfHeight, -radius);
    vec3 maxPoint = position + vec3(+radius, +halfHeight, +radius);

    bool overlaps =
        maxPoint.x >= xBounds.x && minPoint.x <= xBounds.y &&
        maxPoint.y >= yBounds.x && minPoint.y <= yBounds.y &&
        maxPoint.z >= zBounds.x && minPoint.z <= zBounds.y;

    return overlaps;
}

void CapsuleCollider::RenderDebug()
{
    if (Debugger::GetCollisionDebugEnabled())
    {
        Debugger::DrawCapsuleDebug(radius, height, GetWorldPosition());
    }
}

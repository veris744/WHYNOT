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

bool CapsuleCollider::Collides(float _rad1, vec3 _pos1, Hit& hit)
{
    return CheckCircleCircle(radius, GetWorldPosition(), _rad1, _pos1, hit);
}

bool CapsuleCollider::Collides(vec3 _dimensions, vec3 _pos1, Hit& hit, bool isSlope)
{
    if (!isSlope)
        return CheckCapsuleSquare(radius, height, GetWorldPosition(), _dimensions, _pos1, hit);

    return CheckSlopeCapsule(_dimensions, _pos1, radius, height, GetWorldPosition(), hit);
}

bool CapsuleCollider::Collides(float _height, float _radius, vec3 _pos1, Hit& hit)
{
    return CheckCapsuleCapsule(_radius, _height, _pos1, radius, height, GetWorldPosition(), hit);
}

bool CapsuleCollider::Collides(vec2 _dimensions, vec3 _pos1, Hit& hit)
{
    return CheckPlaneCapsule(_dimensions, _pos1,{0, 1, 0}, radius, height, GetWorldPosition(), hit);
}

bool CapsuleCollider::RayCollides(vec3 _rayOrigin, vec3 _rayDir, Hit& hit)
{
    _rayDir = normalize(_rayDir);

    vec3 toCapsule = GetWorldPosition() - _rayOrigin;
    float dotDir = dot(_rayDir, toCapsule);
    if (dotDir < 0)
        return false;

    float halfHeight = (height * 0.5f) - radius;
    vec3 A = GetWorldPosition() + vec3(0, -halfHeight, 0); // bottom of capsule
    vec3 B = GetWorldPosition() + vec3(0, halfHeight, 0);  // top of capsule

    // First check if we're inside the capsule
    vec3 closestOnSeg = closestPointOnSegment(A, B, _rayOrigin);
    float distToCenter = distance(_rayOrigin, closestOnSeg);
    if (distToCenter <= radius) {
        hit.hasHit = true;
        hit.distSQ = 0;
        hit.point = _rayOrigin;
        hit.normal = normalize(_rayOrigin - closestOnSeg);
        hit.type = UI;
        hit.otherEntity = parent;
        return true;
    }

    // Check for intersection with infinite cylinder
    vec3 AB = B - A;
    vec3 AO = _rayOrigin - A;

    float ABdotAB = dot(AB, AB);
    float ABdotRay = dot(AB, _rayDir);
    float ABdotAO = dot(AB, AO);
    float AOdotRay = dot(AO, _rayDir);
    float AOdotAO = dot(AO, AO);

    // Quadratic equation coefficients
    float a = ABdotAB - ABdotRay * ABdotRay;
    float b = ABdotAB * AOdotRay - ABdotAO * ABdotRay;
    float c = ABdotAB * AOdotAO - ABdotAO * ABdotAO - radius * radius * ABdotAB;

    float t_cylinder = FLT_MAX;
    bool cylinder_hit = false;

    float discriminant = b * b - a * c;
    if (discriminant >= 0.0f) {
        float sqrtDisc = sqrt(discriminant);
        float t1 = (-b - sqrtDisc) / a;
        float t2 = (-b + sqrtDisc) / a;

        // Find the first valid intersection in front of the ray
        float t = (t1 > 0) ? ((t2 > 0) ? std::min(t1, t2) : t1) : ((t2 > 0) ? t2 : -1.0f);
        if (t > 0) {
            vec3 point = _rayOrigin + _rayDir * t;
            closestOnSeg = closestPointOnSegment(A, B, point);
            if (distance(point, closestOnSeg) <= radius + 1e-5f) {
                t_cylinder = t;
                cylinder_hit = true;
            }
        }
    }

    // Check for intersection with top sphere
    float t_top = FLT_MAX;
    bool top_hit = RaySphereIntersection(_rayOrigin, _rayDir, B, radius, t_top);

    // Check for intersection with bottom sphere
    float t_bottom = FLT_MAX;
    bool bottom_hit = RaySphereIntersection(_rayOrigin, _rayDir, A, radius, t_bottom);

    // Find the closest valid intersection
    float t_min = FLT_MAX;
    bool has_hit = false;
    vec3 hit_point;
    vec3 hit_normal;

    if (cylinder_hit && t_cylinder < t_min) {
        t_min = t_cylinder;
        hit_point = _rayOrigin + _rayDir * t_cylinder;
        closestOnSeg = closestPointOnSegment(A, B, hit_point);
        hit_normal = normalize(hit_point - closestOnSeg);
        has_hit = true;
    }
    if (top_hit && t_top < t_min) {
        t_min = t_top;
        hit_point = _rayOrigin + _rayDir * t_top;
        hit_normal = normalize(hit_point - B);
        has_hit = true;
    }
    if (bottom_hit && t_bottom < t_min) {
        t_min = t_bottom;
        hit_point = _rayOrigin + _rayDir * t_bottom;
        hit_normal = normalize(hit_point - A);
        has_hit = true;
    }

    if (!has_hit) return false;

    // Fill hit information
    hit.hasHit = true;
    hit.distSQ = t_min * t_min;
    hit.point = hit_point;
    hit.normal = hit_normal;
    hit.type = WorldHit;
    hit.otherEntity = parent;

    return true;
}

bool CapsuleCollider::RaySphereIntersection(vec3 rayOrigin, vec3 rayDir, vec3 sphereCenter, float sphereRadius,
    float& t)
{
    vec3 oc = rayOrigin - sphereCenter;
    float a = dot(rayDir, rayDir);
    float b = 2.0f * dot(oc, rayDir);
    float c = dot(oc, oc) - sphereRadius * sphereRadius;
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) return false;

    float sqrt_discriminant = sqrt(discriminant);
    float t0 = (-b - sqrt_discriminant) / (2.0f * a);
    float t1 = (-b + sqrt_discriminant) / (2.0f * a);

    t = (t0 < t1) ? t0 : t1;
    if (t < 0) {
        t = (t0 < t1) ? t1 : t0;
        if (t < 0) return false;
    }

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
    Debugger::DrawCapsuleDebug(radius, height, GetWorldPosition());
}

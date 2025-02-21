#include "CircleCollider.h"

#include "BoxCollider.h"
#include "Managers/Renderer.h"
#include "Utils/Debugger.h"


bool CircleCollider::Collides(Collider* other) 
{
    return other->Collides(radius, GetWorldPosition());
}

bool CircleCollider::Collides(float _rad1, vec3 _pos1)
{
    return CheckCircleCircle(radius, GetWorldPosition(), _rad1, _pos1);
}

bool CircleCollider::CheckInBounds(const vec2& xBounds, const vec2& yBounds, const vec2& zBounds, bool triggerElement)
{
    bool isInside = true;
    vec3 outNormal = vec3(0.0f, 0.0f, 0.0f);
    vec3 worldPos = GetWorldPosition();

    // Check X bounds
    if (worldPos.x - radius < xBounds.x) {
        outNormal = vec3(1.0f, 0.0f, 0.0f);
        isInside = false;
    } 
    else if (worldPos.x + radius > xBounds.y) {
        outNormal = vec3(-1.0f, 0.0f, 0.0f);
        isInside = false;
    }

    // Check Y bounds
    if (worldPos.y - radius < yBounds.x) {
        outNormal = vec3(0.0f, 1.0f, 0.0f);
        isInside = false;
    } 
    else if (worldPos.y + radius > yBounds.y) {
        outNormal = vec3(0.0f, -1.0f, 0.0f);
        isInside = false;
    }

    // Check Z bounds
    if (worldPos.z - radius < zBounds.x) {
        outNormal = vec3(0.0f, 0.0f, 1.0f);
        isInside = false;
    } 
    else if (worldPos.z + radius > zBounds.y) {
        outNormal = vec3(0.0f, 0.0f, -1.0f);
        isInside = false;
    }
    if (triggerElement && !isInside)
    {
        OnOutOfBoundsDelegate.Execute(outNormal);
    }

    return isInside;
}

bool CircleCollider::OverlapsBounds(const vec2& xBounds, const vec2& yBounds, const vec2& zBounds,
    bool triggerDelegate)
{
    // Get the collider's center and radius
    vec3 center = GetWorldPosition();

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

void CircleCollider::Render()
{
    Debugger::DrawSphereDebug(radius, GetWorldPosition());    
}





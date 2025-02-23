#pragma once
#include "Collider.h"
#include "Reflection/Reflection.h"

class CircleCollider : public Collider
{
public:
    float radius;
    
    CircleCollider(float _radius = 0)
        : radius(_radius)
    {
        type = ColliderType::CIRCLE;
    }
    
    bool Collides(Collider* other) override;
    bool Collides(float _rad1, vec3 _pos1) override;
    bool CheckInBounds(const vec2& xBounds, const vec2& yBounds, const vec2& zBounds, bool triggerDelegate = true) override;
    bool OverlapsBounds(const vec2& xBounds, const vec2& yBounds, const vec2& zBounds, bool triggerDelegate = true) override;
    void RenderDebug() override;
    void Update(float deltaTime) override {};
    
    SingleDelegate<vec3> OnOutOfBoundsDelegate;
    
};
REGISTER_CLASS(CircleCollider, {
    REGISTER_MEMBER(CircleCollider, radius)
});
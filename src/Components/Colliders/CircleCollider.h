#pragma once
#include "Collider.h"

class CircleCollider : public Collider
{
public:
    float radius;
    
    CircleCollider(float _radius = 1)
        : radius(_radius)
    {
        type = ColliderShape::CIRCLE;
    }

    bool Collides(Collider* other, Hit& hit) override;
    bool Collides(float _rad1, vec3 _pos1, Hit& hit) override;
    bool Collides(vec3 _dimensions, vec3 _pos1, Hit& hit, bool isSlope) override;
    bool Collides(float _height, float _radius, vec3 _pos1, Hit& hit) override;
    bool Collides(vec2 _dimensions, vec3 _pos1, Hit& hit) override;
    bool RayCollides(vec3 _rayOrigin, vec3 _rayDir, Hit& hit) override;
    bool CheckInBounds(const vec2& xBounds, const vec2& yBounds, const vec2& zBounds) override;
    bool OverlapsBounds(const vec2& xBounds, const vec2& yBounds, const vec2& zBounds) override;
    void RenderDebug() override;
    void Update(float deltaTime) override {};
    
    
};
REGISTER_CLASS(CircleCollider,
    REGISTER_MEMBER(CircleCollider, name, MemberProperty::Serializable),
    REGISTER_MEMBER(CircleCollider, radius, MemberProperty::Viewable | MemberProperty::Editable | MemberProperty::Serializable),
    REGISTER_MEMBER(CircleCollider, type, MemberProperty::Viewable | MemberProperty::Serializable),
    REGISTER_MEMBER(CircleCollider, profile, MemberProperty::Viewable | MemberProperty::Serializable)
);
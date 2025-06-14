#pragma once
#include "Collider.h"

class CapsuleCollider : public Collider
{
public:
    float height;
    float radius;

    CapsuleCollider(float _height = 1, float _radius = 1)
        : height(_height), radius(_radius)
    {
        type = ColliderShape::CIRCLE;
    }

    bool Collides(Collider* other, Hit& hit) override;
    bool Collides(float _rad1, vec3 _pos1, Hit& hit) override;
    bool Collides(vec3 _dimensions, vec3 _pos1, Hit& hit, bool isSlope) override;
    bool Collides(float _height, float _radius, vec3 _pos1, Hit& hit) override;
    bool Collides(vec2 _dimensions, vec3 _pos1, Hit& hit) override;
    bool RayCollides(vec3 _rayOrigin, vec3 _rayDir, Hit& hit) override;
    bool RaySphereIntersection(vec3 rayOrigin, vec3 rayDir, vec3 sphereCenter, float sphereRadius, float& t);
    bool CheckInBounds(const vec2& xBounds, const vec2& yBounds, const vec2& zBounds) override;
    bool OverlapsBounds(const vec2& xBounds, const vec2& yBounds, const vec2& zBounds) override;
    void RenderDebug() override;
    void Update(float deltaTime) override {};
    
    
};
REGISTER_CLASS(CapsuleCollider,
    REGISTER_MEMBER(CapsuleCollider, name, MemberProperty::Serializable),
    REGISTER_MEMBER(CapsuleCollider, radius, MemberProperty::Viewable | MemberProperty::Editable | MemberProperty::Serializable),
    REGISTER_MEMBER(CapsuleCollider, height, MemberProperty::Viewable | MemberProperty::Editable | MemberProperty::Serializable),
    REGISTER_MEMBER(CapsuleCollider, type, MemberProperty::Viewable | MemberProperty::Serializable),
    REGISTER_MEMBER(CapsuleCollider, profile, MemberProperty::Viewable | MemberProperty::Serializable)
);
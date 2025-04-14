#pragma once
#include "Collider.h"

class SlopeCollider : public Collider
{
public:
    vec3 dimensions ;

    SlopeCollider(vec3 _dimensions = {0,0,0})
        : dimensions(_dimensions)
    {
        type = ColliderShape::BOX;
    }

    bool Collides(Collider* other, Hit& hit) override;
    bool Collides(float _rad1, vec3 _pos1, Hit& hit) override;
    bool Collides(vec3 _dimensions, vec3 _pos1, Hit& hit, bool isSlope) override;
    bool Collides(float _height, float _radius, vec3 _pos1, Hit& hit) override;
    bool Collides(vec2 _dimensions, vec3 _pos1, Hit& hit) override;
    bool RayCollides(vec3 _rayOrigin, vec3 _rayDir, Hit& hit) override;
    bool CheckInBounds(const vec2& xBounds, const vec2& yBounds, const vec2& zBounds, bool triggerDelegate = true) override;
    bool OverlapsBounds(const vec2& xBounds, const vec2& yBounds, const vec2& zBounds, bool triggerDelegate = true) override;
    void RenderDebug() override;
    void Update(float deltaTime) override {};

};
REGISTER_CLASS(SlopeCollider,
    REGISTER_MEMBER(SlopeCollider, name, MemberProperty::Serializable),
    REGISTER_MEMBER(SlopeCollider, dimensions, MemberProperty::Viewable | MemberProperty::Editable | MemberProperty::Serializable),
    REGISTER_MEMBER(SlopeCollider, type, MemberProperty::Viewable | MemberProperty::Serializable),
    REGISTER_MEMBER(SlopeCollider, profile, MemberProperty::Viewable | MemberProperty::Serializable)
);

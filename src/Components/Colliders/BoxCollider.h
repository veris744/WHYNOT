#pragma once
#include "Collider.h"

class BoxCollider : public Collider
{
public:
    vec3 dimensions ;

    BoxCollider(vec3 _dimensions = {0,0,0})
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
REGISTER_CLASS(BoxCollider,
    REGISTER_MEMBER(BoxCollider, name, MemberProperty::Serializable),
    REGISTER_MEMBER(BoxCollider, dimensions, MemberProperty::Viewable | MemberProperty::Editable | MemberProperty::Serializable),
    REGISTER_MEMBER(BoxCollider, type, MemberProperty::Viewable | MemberProperty::Serializable),
    REGISTER_MEMBER(BoxCollider, profile, MemberProperty::Viewable | MemberProperty::Serializable)
);

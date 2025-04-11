#pragma once
#include "Collider.h"

class PlaneCollider : public Collider
{
public:
    vec2 dimensions ;

    PlaneCollider(vec3 _dimensions = {0,0,0})
        : dimensions(_dimensions)
    {
        type = ColliderShape::BOX;
    }

    bool Collides(Collider* other, Hit& hit) override;
    bool Collides(float _rad1, vec3 _pos1, Hit& hit) override;
    bool Collides(vec3 _dimensions, vec3 _pos1, Hit& hit) override;
    bool Collides(float _height, float _radius, vec3 _pos1, Hit& hit) override;
    bool Collides(vec2 _dimensions, vec3 _pos1, Hit& hit) override;
    bool RayCollides(vec3 _rayOrigin, vec3 _rayDir, Hit& hit) override;
    bool CheckInBounds(const vec2& xBounds, const vec2& yBounds, const vec2& zBounds, bool triggerDelegate = true) override;
    bool OverlapsBounds(const vec2& xBounds, const vec2& yBounds, const vec2& zBounds, bool triggerDelegate = true) override;
    void RenderDebug() override;
    void Update(float deltaTime) override {};

};
REGISTER_CLASS(PlaneCollider,
    REGISTER_MEMBER(PlaneCollider, name, MemberProperty::Serializable),
    REGISTER_MEMBER(PlaneCollider, dimensions, MemberProperty::Viewable | MemberProperty::Editable | MemberProperty::Serializable),
    REGISTER_MEMBER(PlaneCollider, type, MemberProperty::Viewable | MemberProperty::Serializable),
    REGISTER_MEMBER(PlaneCollider, profile, MemberProperty::Viewable | MemberProperty::Serializable)
);

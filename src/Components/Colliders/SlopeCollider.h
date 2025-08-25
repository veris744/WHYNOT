#pragma once
#include "Collider.h"

class SlopeCollider : public Collider
{
public:
    glm::vec3 dimensions ;

    SlopeCollider(glm::vec3 _dimensions = {0,0,0})
        : dimensions(_dimensions)
    {
        type = ColliderShape::BOX;
    }

    bool Collides(Collider* other, Hit& hit) override;
    bool Collides(float _rad1, glm::vec3 _pos1, Hit& hit) override;
    bool Collides(glm::vec3 _dimensions, glm::vec3 _pos1, Hit& hit, bool isSlope) override;
    bool Collides(float _height, float _radius, glm::vec3 _pos1, Hit& hit) override;
    bool Collides(glm::vec2 _dimensions, glm::vec3 _pos1, Hit& hit) override;
    bool RayCollides(glm::vec3 _rayOrigin, glm::vec3 _rayDir, Hit& hit) override;
    bool CheckInBounds(const glm::vec2& xBounds, const glm::vec2& yBounds, const glm::vec2& zBounds) override;
    bool OverlapsBounds(const glm::vec2& xBounds, const glm::vec2& yBounds, const glm::vec2& zBounds) override;
    void RenderDebug() override;
    void Update(float deltaTime) override {};

};
REGISTER_CLASS(SlopeCollider,
    REGISTER_MEMBER(SlopeCollider, name, MemberProperty::Serializable),
    REGISTER_MEMBER(SlopeCollider, dimensions, MemberProperty::Viewable | MemberProperty::Editable | MemberProperty::Serializable),
    REGISTER_MEMBER(SlopeCollider, type, MemberProperty::Viewable | MemberProperty::Serializable),
    REGISTER_MEMBER(SlopeCollider, profile, MemberProperty::Viewable | MemberProperty::Serializable)
);

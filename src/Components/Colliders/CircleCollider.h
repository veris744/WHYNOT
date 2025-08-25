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
REGISTER_CLASS(CircleCollider,
    REGISTER_MEMBER(CircleCollider, name, MemberProperty::Serializable),
    REGISTER_MEMBER(CircleCollider, radius, MemberProperty::Viewable | MemberProperty::Editable | MemberProperty::Serializable),
    REGISTER_MEMBER(CircleCollider, type, MemberProperty::Viewable | MemberProperty::Serializable),
    REGISTER_MEMBER(CircleCollider, profile, MemberProperty::Viewable | MemberProperty::Serializable)
);
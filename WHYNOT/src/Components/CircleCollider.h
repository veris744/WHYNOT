#pragma once
#include "Collider.h"

class CircleCollider : public Collider
{
    float radius;

    
public:
    CircleCollider(float _radius)
        : radius(_radius)
    {
        type = ColliderType::CIRCLE;
    }
    
    bool Collides(const std::shared_ptr<Collider>& other) const override;
    bool Collides(float _rad1, vec3 _pos1) const override;
    bool Collides(vec3 _width1, vec3 _height1, vec3 _depth1, vec3 _pos1) const override;
    bool CheckInBounds(const vec2& xBounds, const vec2& yBounds, const vec2& zBounds) override;
    void Render() override;
    void Update(float deltaTime) override;

    float GetRadius() const { return radius; }
    
    SingleDelegate<vec3> OnOutOfBoundsDelegate;
    
};

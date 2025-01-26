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
    
    bool Collides(const CircleCollider& other) override;
    bool Collides(const BoxCollider& other) override;
    bool CheckInBounds() override;
    void Render() override;
    void Update(float deltaTime) override;

    float GetRadius() const { return radius; }
    
    SingleDelegate<vec3> OnOutOfBoundsDelegate;
};

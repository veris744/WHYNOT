#pragma once
#include "Collider.h"

class BoxCollider : public Collider
{
    float width;
    float height;
    float depth;
    
public:
    BoxCollider(float _width, float _height, float _depth)
        : width(_width), height(_height), depth(_depth)
    {
        type = ColliderType::BOX;
    }
    
    bool Collides(const CircleCollider& other) override;
    bool Collides(const BoxCollider& other) override;
    bool CheckInBounds() override;
    void Render() override;
    void Update(float deltaTime) override;

    vec3 GetSize() const { return vec3(width, height, depth); }
    
};

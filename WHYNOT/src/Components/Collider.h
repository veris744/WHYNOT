#pragma once
#include "Component.h"
#include "Transform.h"
#include "Entities/Entity.h"

class BoxCollider;
class CircleCollider;

enum class ColliderType
{
    CIRCLE, BOX
};

class Collider : public Component
{
protected:
    ColliderType type;

    bool CheckCircleCircle(float _rad1, vec3 _pos1, float _rad2, vec3 _pos2);
    bool CheckCirclePlane(float _rad1, vec3 _pos1, vec3 _point, vec3 _normal);
    bool CheckCircleSquare(float _rad1, vec3 _pos1, vec3 _width2, vec3 _height2, vec3 _depth, vec3 _pos2);
    bool CheckSquareSquare(vec3 _width1, vec3 _height1, vec3 _pos1, vec3 _width2, vec3 _height2, vec3 _pos2);
    
public:
    Collider()
    {
        name = "COLLIDER";
    }
    virtual ~Collider() = default;
    
    vec3 position;

    ColliderType GetType() const {  return type; }
    
    virtual bool Collides(const CircleCollider& other) = 0;
    virtual bool Collides(const BoxCollider& other) = 0;
    virtual bool CheckInBounds(const vec2& xBounds, const vec2& yBounds, const vec2& zBounds) = 0;
    virtual void Render() = 0;
};

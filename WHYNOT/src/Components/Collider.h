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

    Transform* transform = nullptr;
   
    
public:
    Collider()
    {
        name = "COLLIDER";
    }
    virtual ~Collider() = default;

    vec3 position = vec3(0, 0, 0);

    ColliderType GetType() const {  return type; }

    vec3 GetWorldPosition();

    bool CheckCircleCircle(float _rad1, vec3 _pos1, float _rad2, vec3 _pos2) const;
    bool CheckCirclePlane(float _rad1, vec3 _pos1, vec3 _point, vec3 _normal) const;
    bool CheckCircleSquare(float _rad1, vec3 _pos1, vec3 _width2, vec3 _height2, vec3 _depth, vec3 _pos2) const;
    bool CheckSquareSquare(vec3 _width1, vec3 _height1, vec3 _depth1, vec3 _pos1, vec3 _width2, vec3 _height2, vec3 _depth2, vec3 _pos2) const;
    
    virtual bool Collides(float _rad1, vec3 _pos1) = 0;
    virtual bool Collides(Collider* other) = 0;
    virtual bool CheckInBounds(const vec2& xBounds, const vec2& yBounds, const vec2& zBounds, bool triggerDelegate = true) = 0;
    virtual bool OverlapsBounds(const vec2& xBounds, const vec2& yBounds, const vec2& zBounds, bool triggerDelegate = true) = 0;

    virtual void Render() = 0;

    SingleDelegate<const std::shared_ptr<Entity>&, vec3> CollisionDelegate;
};

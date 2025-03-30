#pragma once
#include "Transform.h"
#include "Physics/CollisionPresets.h"
#include "Physics/Hit.h"
#include "Utils/SingleDelegate.h"

struct Hit;

enum class ColliderShape
{
    CIRCLE, BOX
};
REGISTER_ENUM(ColliderShape,
    {"CIRCLE", ColliderShape::CIRCLE},
    {"BOX", ColliderShape::BOX}
);

class Collider : public Component
{
protected:
    Transform* transform = nullptr;

public:
    ColliderShape type;
    Collider()
    {
        name = "COLLIDER";
    }
    virtual ~Collider() = default;

    vec3 position = vec3(0, 0, 0);

    ColliderShape GetType() const {  return type; }

    vec3 GetWorldPosition();
    void Initialize() override;

    bool CheckCircleCircle(float _rad1, vec3 _pos1, float _rad2, vec3 _pos2) const;
    bool CheckCirclePlane(float _rad1, vec3 _pos1, vec3 _point, vec3 _normal) const;
    bool CheckCircleSquare(float _rad1, vec3 _pos1, vec3 _width2, vec3 _height2, vec3 _depth, vec3 _pos2) const;
    bool CheckSquareSquare(vec3 _width1, vec3 _height1, vec3 _depth1, vec3 _pos1, vec3 _width2, vec3 _height2, vec3 _depth2, vec3 _pos2) const;
    
    virtual bool Collides(float _rad1, vec3 _pos1) = 0;
    virtual bool Collides(Collider* other) = 0;
    virtual bool Collides(vec3 _rayOrigin, vec3 _rayDir, Hit& hit) = 0;
    virtual bool CheckInBounds(const vec2& xBounds, const vec2& yBounds, const vec2& zBounds, bool triggerDelegate = true) = 0;
    virtual bool OverlapsBounds(const vec2& xBounds, const vec2& yBounds, const vec2& zBounds, bool triggerDelegate = true) = 0;
    
    SingleDelegate<const std::shared_ptr<Entity>&, vec3> CollisionDelegate;
    SingleDelegate<vec3> OnOutOfBoundsDelegate;
    SingleDelegate<> OnClickedDelegate;

    CollisionProfile profile;
};

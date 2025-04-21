#pragma once
#include "Components/Movement.h"
#include "Components/Transform.h"
#include "Physics/CollisionPresets.h"
#include "Physics/Hit.h"
#include "Utils/SingleDelegate.h"

struct Hit;

enum class ColliderShape
{
    CIRCLE, BOX, CAPSULE, PLANE, SLOPE
};
REGISTER_ENUM(ColliderShape,
    {"CIRCLE", ColliderShape::CIRCLE},
    {"BOX", ColliderShape::BOX},
    {"CAPSULE", ColliderShape::CAPSULE},
    {"PLANE", ColliderShape::PLANE},
    {"SLOPE", ColliderShape::SLOPE}
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

    bool CheckCircleCircle(float _radCircle, vec3 _posCircle, float _radCircle2, vec3 _posCircle2, Hit& hit) const;
    bool CheckCircleSquare(float _radCircle, vec3 _posCircle, vec3 _dimensionsSquare, vec3 _posSquare, Hit& hit) const;
    bool CheckSquareSquare(vec3 _dimensionsSquare, vec3 _posSquare, vec3 _dimensionsSquare2, vec3 _posSquare2, Hit& hit) const;
    bool CheckCapsuleCircle(float _radCapsule, float _heightCapsule, vec3 _posCapsule, float _radCircle, vec3 _posCircle, Hit& hit) const;
    bool CheckCapsuleSquare(float _radCapsule, float _heightCapsule, vec3 _posCapsule, vec3 _dimensionsSquare, vec3 _posSquare, Hit& hit) const;
    bool CheckCapsuleCapsule(float _radCapsule, float _heightCapsule, vec3 _posCapsule, float _radCapsule2, float _heightCapsule2, vec3 _posCapsule2, Hit& hit) const;
    bool CheckPlaneCircle(vec2 _dimensionsPlane, vec3 _posPlane, vec3 _normalPlane, float _radCircle, vec3 _posCircle, Hit& hit) const;
    bool CheckPlaneSquare(vec2 _dimensionsPlane, vec3 _posPlane, vec3 _normalPlane, vec3 _dimensionsSquare, vec3 _posSquare, Hit& hit) const;
    bool CheckPlaneCapsule(vec2 _dimensionsPlane, vec3 _posPlane, vec3 _normalPlane, float _radCapsule, float _heightCapsule, vec3 _posCapsule, Hit& hit) const;
    bool CheckSlopeCircle(vec3 _dimensionsSlope, vec3 _posSlope, float _radCircle, vec3 _posCircle, Hit& hit) const;
    bool CheckSlopeSquare(vec3 _dimensionsSlope, vec3 _posSlope, vec3 _dimensionsSquare, vec3 _posSquare, Hit& hit) const;
    bool CheckSlopeCapsule(vec3 _dimensionsSlope, vec3 _posSlope, float _radCapsule, float _heightCapsule, vec3 _posCapsule, Hit& hit) const;
    bool CheckSlopePlane(vec3 _dimensionsSlope, vec3 _posSlope, vec2 _dimensionsPlane2, vec3 _posPlane2, Hit& hit) const;
    bool CheckSlopeSlope(vec3 _dimensionsSlope, vec3 _posSlope, vec3 _dimensionsSlope2, vec3 _posSlope2, Hit& hit) const;

    virtual bool Collides(float _rad1, vec3 _pos1, Hit& hit) = 0;
    virtual bool Collides(vec3 dimensions, vec3 _pos1, Hit& hit, bool isSlope = false) = 0;
    virtual bool Collides(float _height, float radius, vec3 _pos1, Hit& hit) = 0;
    virtual bool Collides(vec2 dimensions, vec3 _pos1, Hit& hit) = 0;
    virtual bool Collides(Collider* other, Hit& hit) = 0;
    virtual bool RayCollides(vec3 _rayOrigin, vec3 _rayDir, Hit& hit) = 0;
    virtual bool CheckInBounds(const vec2& xBounds, const vec2& yBounds, const vec2& zBounds, bool triggerDelegate = true) = 0;
    virtual bool OverlapsBounds(const vec2& xBounds, const vec2& yBounds, const vec2& zBounds, bool triggerDelegate = true) = 0;
    
    SingleDelegate<Entity*, vec3> CollisionDelegate;
    SingleDelegate<vec3> OnOutOfBoundsDelegate;
    SingleDelegate<> OnClickedDelegate;

    CollisionProfile profile;

    Movement* movement = nullptr;
    void UpdateMovement(vec3 _normal);
};

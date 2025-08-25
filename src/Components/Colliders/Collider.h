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

    glm::vec3 position = glm::vec3(0, 0, 0);

    ColliderShape GetType() const {  return type; }

    glm::vec3 GetWorldPosition();
    void Initialize() override;

    bool CheckCircleCircle(float _radCircle, glm::vec3 _posCircle, float _radCircle2, glm::vec3 _posCircle2, Hit& hit) const;
    bool CheckCircleSquare(float _radCircle, glm::vec3 _posCircle, glm::vec3 _dimensionsSquare, glm::vec3 _posSquare, Hit& hit) const;
    bool CheckSquareSquare(glm::vec3 _dimensionsSquare, glm::vec3 _posSquare, glm::vec3 _dimensionsSquare2, glm::vec3 _posSquare2, Hit& hit) const;
    bool CheckCapsuleCircle(float _radCapsule, float _heightCapsule, glm::vec3 _posCapsule, float _radCircle, glm::vec3 _posCircle, Hit& hit) const;
    bool CheckCapsuleSquare(float _radCapsule, float _heightCapsule, glm::vec3 _posCapsule, glm::vec3 _dimensionsSquare, glm::vec3 _posSquare, Hit& hit) const;
    bool CheckCapsuleCapsule(float _radCapsule, float _heightCapsule, glm::vec3 _posCapsule, float _radCapsule2, float _heightCapsule2, glm::vec3 _posCapsule2, Hit& hit) const;
    bool CheckPlaneCircle(glm::vec2 _dimensionsPlane, glm::vec3 _posPlane, glm::vec3 _normalPlane, float _radCircle, glm::vec3 _posCircle, Hit& hit) const;
    bool CheckPlaneSquare(glm::vec2 _dimensionsPlane, glm::vec3 _posPlane, glm::vec3 _normalPlane, glm::vec3 _dimensionsSquare, glm::vec3 _posSquare, Hit& hit) const;
    bool CheckPlaneCapsule(glm::vec2 _dimensionsPlane, glm::vec3 _posPlane, glm::vec3 _normalPlane, float _radCapsule, float _heightCapsule, glm::vec3 _posCapsule, Hit& hit) const;
    bool CheckSlopeCircle(glm::vec3 _dimensionsSlope, glm::vec3 _posSlope, float _radCircle, glm::vec3 _posCircle, Hit& hit) const;
    bool CheckSlopeSquare(glm::vec3 _dimensionsSlope, glm::vec3 _posSlope, glm::vec3 _dimensionsSquare, glm::vec3 _posSquare, Hit& hit) const;
    bool CheckSlopeCapsule(glm::vec3 _dimensionsSlope, glm::vec3 _posSlope, float _radCapsule, float _heightCapsule, glm::vec3 _posCapsule, Hit& hit) const;
    bool CheckSlopePlane(glm::vec3 _dimensionsSlope, glm::vec3 _posSlope, glm::vec2 _dimensionsPlane2, glm::vec3 _posPlane2, Hit& hit) const;
    bool CheckSlopeSlope(glm::vec3 _dimensionsSlope, glm::vec3 _posSlope, glm::vec3 _dimensionsSlope2, glm::vec3 _posSlope2, Hit& hit) const;

    virtual bool Collides(float _rad1, glm::vec3 _pos1, Hit& hit) = 0;
    virtual bool Collides(glm::vec3 dimensions, glm::vec3 _pos1, Hit& hit, bool isSlope = false) = 0;
    virtual bool Collides(float _height, float radius, glm::vec3 _pos1, Hit& hit) = 0;
    virtual bool Collides(glm::vec2 dimensions, glm::vec3 _pos1, Hit& hit) = 0;
    virtual bool Collides(Collider* other, Hit& hit) = 0;
    virtual bool RayCollides(glm::vec3 _rayOrigin, glm::vec3 _rayDir, Hit& hit) = 0;
    virtual bool CheckInBounds(const glm::vec2& xBounds, const glm::vec2& yBounds, const glm::vec2& zBounds) = 0;
    virtual bool OverlapsBounds(const glm::vec2& xBounds, const glm::vec2& yBounds, const glm::vec2& zBounds) = 0;
    
    SingleDelegate<Entity*, glm::vec3> CollisionDelegate;

    CollisionProfile profile;

    Movement* movement = nullptr;
    void UpdateMovement(const Hit& hit);
};

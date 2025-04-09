#pragma once

#include "Component.h"
#include "Reflection/Reflection.h"

class Transform;



class Movement : public Component
{
    Transform* transform = nullptr;
    
public:
    vec3 speed;
    vec3 acceleration;
    float maxSpeed;
    float maxAcceleration;
    vec3 accumulatedForce = vec3(0);
    bool usesPhysics = false;
    bool isAffectedByGravity = false;
    std::vector<vec3> collisionNormals;
    float mass = 1.0f;

    Movement(): speed(vec3(0)), acceleration(vec3(0)), maxSpeed(10), maxAcceleration(10)
    {
        name = "MOVEMENT";
    }
    ~Movement() override = default;

    void Update(float deltaTime) override;
    void QuickStop()
    {
        speed = vec3(0, 0, 0);
        acceleration = vec3(0, 0, 0);
    }
    void AddForce(vec3 force);
    void ResetForces();
    void AddImpulse(vec3 impulse);

    bool IsGrounded() const
    {
        float y = 0;
        for (const auto& normal : collisionNormals)
        {
            y += normal.y;
        }
        return y > 0;
    }
};

REGISTER_CLASS(Movement,
    REGISTER_MEMBER(Movement, name, MemberProperty::Serializable),
    REGISTER_MEMBER(Movement, maxSpeed, MemberProperty::Viewable | MemberProperty::Editable | MemberProperty::Serializable),
    REGISTER_MEMBER(Movement, maxAcceleration, MemberProperty::Viewable | MemberProperty::Editable | MemberProperty::Serializable),
    REGISTER_MEMBER(Movement, speed, MemberProperty::Viewable | MemberProperty::Editable | MemberProperty::Serializable),
    REGISTER_MEMBER(Movement, acceleration, MemberProperty::Viewable | MemberProperty::Editable | MemberProperty::Serializable),
    REGISTER_MEMBER(Movement, isAffectedByGravity, MemberProperty::Viewable | MemberProperty::Editable | MemberProperty::Serializable),
    REGISTER_MEMBER(Movement, usesPhysics, MemberProperty::Viewable | MemberProperty::Editable | MemberProperty::Serializable)
);
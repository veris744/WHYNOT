#pragma once
#include <includes.h>

#include "Component.h"
#include "Reflection/Reflection.h"

class Transform;



class Movement : public Component
{
    Transform* transform;
    
public:
    vec3 speed;
    vec3 acceleration;
    float maxSpeed;
    float maxAcceleration;

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
};

REGISTER_CLASS(Movement, {
    REGISTER_MEMBER(Movement, maxSpeed, MemberProperty::Viewable),
    REGISTER_MEMBER(Movement, maxAcceleration, MemberProperty::Viewable),
    REGISTER_MEMBER(Movement, speed, MemberProperty::Viewable),
    REGISTER_MEMBER(Movement, acceleration, MemberProperty::Viewable)
});
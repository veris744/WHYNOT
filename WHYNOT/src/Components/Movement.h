#pragma once
#include <includes.h>

#include "Component.h"
#include "Reflection/Reflection.h"

class Transform;



class Movement : public Component
{
    std::shared_ptr<Transform> transform;
    
public:
    vec3 speed;
    vec3 acceleration;
    float maxSpeed;
    float maxAcceleration;

    Movement(): speed(vec3(0)), acceleration(vec3(0)), maxSpeed(10), maxAcceleration(10)
    {
        name = "MOVEMENT";
    }

    void Update(float deltaTime);
    void QuickStop()
    {
        speed = vec3(0, 0, 0);
        acceleration = vec3(0, 0, 0);
    }
};

REGISTER_CLASS(Movement, {
    REGISTER_MEMBER(Movement, maxSpeed),
    REGISTER_MEMBER(Movement, maxAcceleration),
    REGISTER_MEMBER(Movement, speed),
    REGISTER_MEMBER(Movement, acceleration)
});
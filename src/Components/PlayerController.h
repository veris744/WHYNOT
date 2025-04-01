#pragma once

#include "Component.h"
#include "Reflection/Reflection.h"

class Transform;

class PlayerController : public Component
{
    vec3 speed = vec3(0);
    vec3 acceleration = vec3(0);
    vec3 currentInput = vec3(0);
    Transform* transform = nullptr;
    bool bCanShoot = false;

    
public:
    PlayerController(): transform(nullptr)
    {
        name = "PLAYERCONTROLLER";
    }
    ;
    
    float maxSpeed = 5;
    float maxAcceleration = 5;
    float accelerationRate = 2;
    float decelerationRate = 20;

    void StopMovement()
    {
        speed = vec3(0, 0, 0);
        acceleration = vec3(0, 0, 0);
    }

    void SetSpeed(vec3 _speed)
    {
        speed = _speed;
    }
    void SetAccel(vec3 _acceleration)
    {
        acceleration = _acceleration;
    }
    void SetInput(vec3 _input)
    {
        currentInput = _input;
    }
    
    void StopForwardMovement();
    void StopRightMovement();
    void StopUpMovement();

    vec3 GetSpeed() const
    {
        return speed;
    }
    vec3 GetAccel() const
    {
        return acceleration;
    }
    vec3 GetInput() const
    {
        return currentInput;
    }
    void SetCanShoot(bool _canShoot) { bCanShoot = _canShoot; }

    void Update(float deltaTime) override;
    void Shoot();
};
REGISTER_CLASS(PlayerController,
    REGISTER_MEMBER(PlayerController, name, MemberProperty::Serializable),
    REGISTER_MEMBER(PlayerController, maxSpeed, MemberProperty::Viewable | MemberProperty::Editable | MemberProperty::Serializable),
    REGISTER_MEMBER(PlayerController, maxAcceleration, MemberProperty::Viewable | MemberProperty::Editable | MemberProperty::Serializable),
    REGISTER_MEMBER(PlayerController, accelerationRate, MemberProperty::Viewable | MemberProperty::Editable | MemberProperty::Serializable),
    REGISTER_MEMBER(PlayerController, decelerationRate, MemberProperty::Viewable | MemberProperty::Editable | MemberProperty::Serializable)
)
#pragma once

#include "Component.h"

class Transform;

class PlayerController : public Component
{
    vec3 speed;
    vec3 acceleration;
    vec3 currentInput;
    std::shared_ptr<Transform> transform;

    bool isPositionLocked = false;

    
public:
    PlayerController() = default;
    
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

    void Update(float deltaTime) override;
    void Shoot();
    void SetPositionLocked(bool _positionLocked) { isPositionLocked = _positionLocked; }
};

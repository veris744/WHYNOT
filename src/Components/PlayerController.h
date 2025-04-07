#pragma once

#include "Component.h"
#include "Movement.h"
#include "Reflection/Reflection.h"

class Transform;

class PlayerController : public Component
{
    vec3 currentInput = vec3(0);
    Transform* transform = nullptr;
    Movement* movement = nullptr;
    bool bCanShoot = false;
    bool freeMode = false;
    
public:
    PlayerController()
    {
        name = "PLAYERCONTROLLER";
    }

    float accelerationRate = 20;
    //float decelerationRate = 15;

    void SetInput(vec3 _input)
    {
        currentInput = _input;
    }

    vec3 GetInput() const
    {
        return currentInput;
    }

    void Update(float deltaTime) override;
    void UpdateMovement(float deltaTime) const;
    void EnterFreeMode(bool _freeMode);
    void Shoot();
    void SetCanShoot(bool _canShoot) { bCanShoot = _canShoot; }
};
REGISTER_CLASS(PlayerController,
    REGISTER_MEMBER(PlayerController, accelerationRate, MemberProperty::Viewable | MemberProperty::Editable ),
)
#pragma once

#include "Component.h"
#include "Movement.h"
#include "Reflection/Reflection.h"

class Transform;

class PlayerController : public Component
{
    Transform* transform = nullptr;
    Movement* movement = nullptr;
    PhysicsMaterial* physicsMat = nullptr;
    bool bCanShoot = false;
    bool freeMode = false;
    
public:
    PlayerController()
    {
        name = "PLAYERCONTROLLER";
    }

    float accelerationRate = 20;
    float decelerationRate = 30;

    vec3 currentInput = vec3(0);

    void Update(float deltaTime) override;
    void UpdateMovement(float deltaTime) const;
    void EnterFreeMode(bool _freeMode);
    void Shoot();
    void Jump();
    void SetCanShoot(bool _canShoot) { bCanShoot = _canShoot; }
};
REGISTER_CLASS(PlayerController,
    REGISTER_MEMBER(PlayerController, accelerationRate, MemberProperty::Viewable | MemberProperty::Editable ),
)
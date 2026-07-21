#pragma once

#include "Component.h"
#include "Movement.h"
#include "Reflection/Reflection.h"

class Transform;

class PlayerController : public Component
{
protected:

    Transform* transform = nullptr;
    Movement* movement = nullptr;
    PhysicsMaterial* physicsMat = nullptr;
    bool freeMode = false;
    bool canJump = false;
    
public:
    PlayerController()
    {
        name = "PLAYERCONTROLLER";
    }

    float accelerationRate = 20;
    float decelerationRate = 30;

    glm::vec3 currentInput = glm::vec3(0);

    virtual void Update(float deltaTime) override;
    virtual void ProcessInput(int key, bool press);
    virtual void UpdateMovement(float deltaTime) const;
    virtual void EnterFreeMode(bool _freeMode);
    virtual void Jump();
};
REGISTER_CLASS(PlayerController,
    REGISTER_MEMBER(PlayerController, accelerationRate, MemberProperty::Viewable | MemberProperty::Editable ),
    REGISTER_MEMBER(PlayerController, decelerationRate, MemberProperty::Viewable | MemberProperty::Editable )
)
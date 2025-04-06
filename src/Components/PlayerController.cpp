#include "PlayerController.h"

#include <Managers/ConfigurationValues.h>
#include <Managers/World.h>
#include <Utils/Parser.h>

#include "GameManagers/AliensLogic.h"
#include "Utils/Timer.h"
#include "Components/Transform.h"
#include "Components/Movement.h"


void PlayerController::Update(float deltaTime)
{
    if (!transform)
    {
        transform = parent->GetComponent<Transform>();
    }
    if (!movement)
    {
        movement = parent->GetComponent<Movement>();
    }
    if (ConfigurationValues::CanPlayerMove)
    {
        if (!freeMode)
        {
            currentInput = vec3(currentInput.x, 0, currentInput.z);
        }
        UpdateMovement(deltaTime);
    }
}

void PlayerController::UpdateMovement(float deltaTime) const
{
    vec3 direction = vec3(0);

    if (length(currentInput) > 0.0f)
    {
        vec3 inputDir = normalize(currentInput);
        if (freeMode)
        {
            direction = inputDir.x * transform->right + inputDir.y * transform->up + inputDir.z * transform->forward;
        }
        else
        {
            direction = inputDir.x * transform->right + inputDir.z * transform->forward;
            direction = vec3(direction.x, 0, direction.z);
        }
    }

    vec3 acceleration = vec3(0, 0, 0);
    // Compute the change needed per axis
    for (int i = 0; i < 3; ++i)
    {
        float inputComponent = direction[i];
        float speedComponent = movement->speed[i];

        if (std::abs(inputComponent) > 0.01f)
        {
            acceleration[i] = inputComponent * deltaTime;
        }
        else if (std::abs(speedComponent) > 1.0f)
        {
            acceleration[i] = -sign(speedComponent) * deltaTime;
        }
        else
        {
            acceleration[i] = 0.0f;
            movement->speed[i] = 0.0f;
        }
    }
    if (length(acceleration) > 0.0f)
    {
        acceleration = normalize(acceleration);
        movement->acceleration += acceleration * accelerationRate;
    }
    else
    {
        movement->acceleration = vec3(0, 0, 0);
    }

    Logger::Log(LogLevel::Warning, Parser::Parse(movement->acceleration));
    Logger::Log(LogLevel::Info, Parser::Parse(movement->speed));
}

void PlayerController::EnterFreeMode(bool _freeMode)
{
    //movement->usesPhysics = !_freeMode;
    //movement->isAffectedByGravity = !_freeMode;
    freeMode = _freeMode;
}

void PlayerController::Shoot()
{
    if (World::GetInstance()->IsPaused()) return;
    if (!bCanShoot) return;
    World::GetGameManager()->OnPlayerShootDelegate.Execute();
    bCanShoot = false;
    Timer::StartTimer(0.3f, this, &PlayerController::SetCanShoot, true);
}

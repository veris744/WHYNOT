#include "PlayerController.h"

#include <Managers/ConfigurationValues.h>
#include <Managers/World.h>
#include <Utils/Parser.h>

#include "GameManagers/AliensLogic.h"
#include "Utils/Timer.h"
#include "Components/Transform.h"
#include "Components/Movement.h"


float moveToward(float current, float target, float maxDelta)
{
    if (abs(current - target) <= maxDelta)
        return target;
    return current + sign(target - current) * maxDelta;
}

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
    // Handle player input movement
    vec3 inputDirection = vec3(0);

    if (length(currentInput) > 0.0f)
    {
        vec3 inputDir = normalize(currentInput);
        if (freeMode)
        {
            inputDirection = inputDir.x * transform->right + inputDir.y * transform->up + inputDir.z * transform->forward;
        }
        else
        {
            inputDirection = inputDir.x * transform->right + inputDir.z * transform->forward;
            inputDirection = vec3(inputDirection.x, 0, inputDirection.z);
        }
    }

    // Calculate desired velocity based on input
    vec3 desiredVelocity = vec3(0);
    if (length(inputDirection) > 0.0f)
    {
        desiredVelocity = normalize(inputDirection) * movement->maxSpeed;
    }

    // Apply acceleration toward desired velocity
    for (int i = 0; i < 3; ++i)
    {
        // Skip Y-axis acceleration if not in free mode (let gravity handle it)
        if (i == 1 && !freeMode) continue;

        if (std::abs(desiredVelocity[i]) > 0.01f)
        {
            // Accelerate toward desired velocity
            movement->speed[i] = moveToward(
                movement->speed[i],
                desiredVelocity[i],
                accelerationRate * deltaTime
            );
        }
        else
        {
            // Decelerate when no input
            movement->speed[i] = moveToward(
                movement->speed[i],
                0.0f,
                decelerationRate * deltaTime
            );
        }
    }
}

void PlayerController::EnterFreeMode(bool _freeMode)
{
    //movement->usesPhysics = !_freeMode;
    //movement->isAffectedByGravity = !_freeMode;
    freeMode = _freeMode;
    if (freeMode && movement)
    {
        movement->usesPhysics = !_freeMode;
        movement->physicsProperties.hasGravity = !_freeMode;
    }
}

void PlayerController::Shoot()
{
    if (World::GetInstance()->IsPaused()) return;
    if (!bCanShoot) return;
    World::GetGameManager()->OnPlayerShootDelegate.Execute();
    bCanShoot = false;
    Timer::StartTimer(0.3f, this, &PlayerController::SetCanShoot, true);
}

void PlayerController::Jump()
{
    if (ConfigurationValues::CanPlayerMove && movement->IsGrounded())
        movement->AddImpulse(vec3(0, 7, 0));
}

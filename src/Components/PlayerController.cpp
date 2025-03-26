#include "PlayerController.h"

#include <Managers/ConfigurationValues.h>

#include "Transform.h"
#include "Entities/Entity.h"
#include "Minigame1/AliensLogic.h"
#include "Utils/Parser.h"
#include "Utils/Timer.h"

void PlayerController::StopForwardMovement()
{
    speed = speed - dot(speed, transform->forward) * transform->forward;
    acceleration = acceleration - dot(acceleration, transform->forward) * transform->forward;
}
void PlayerController::StopRightMovement()
{
    speed = speed - dot(speed, transform->right) * transform->right;
    acceleration = acceleration - dot(acceleration, transform->right) * transform->right;
}

void PlayerController::StopUpMovement()
{
    speed = speed - dot(speed, transform->up) * transform->up;
    acceleration = acceleration - dot(acceleration, transform->up) * transform->up;
}

void PlayerController::Update(float deltaTime)
{
    if (!transform)
    {
        transform = parent->GetComponent<Transform>();
    }
    // Logger::Log(LogLevel::Warning, Parser::Parse(transform->rotation.vector()));
    // Logger::Log(LogLevel::Info, Parser::Parse(transform->forward));

    if (!ConfigurationValues::CanPlayerMove)   return;
    
    if (length(currentInput) > 0.0f) {
        acceleration = currentInput * accelerationRate;
        if (length(acceleration) > maxAcceleration)
        {
            acceleration = normalize(acceleration) * maxAcceleration;
        }
        speed += acceleration * deltaTime;

        if (length(speed) > maxSpeed) {
            speed = normalize(speed) * maxSpeed;
        }
    }
    else {
        if (length(speed) > 0.0f) {
            vec3 deceleration = -normalize(speed) * decelerationRate;
            speed += deceleration * deltaTime;

            if (length(speed) < 0.1f) {
                speed = vec3(0.0f);
            }
        }
    }

    vec3 localSpeed = transform->forward * speed.z
     + transform->right * speed.x
    + transform->up * speed.y;
    transform->position = transform->position + localSpeed * deltaTime;
}

void PlayerController::Shoot()
{
    if (!bCanShoot) return;
    AliensLogic::GetInstance()->ShootProjectile();
    bCanShoot = false;
    Timer::StartTimer(0.3f, this, &PlayerController::SetCanShoot, true);
}

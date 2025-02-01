#include "PlayerController.h"

#include "Transform.h"
#include "Entities/Entity.h"
#include "Entities/Projectile.h"
#include "Managers/World.h"

void PlayerController::StopForwardMovement()
{
    speed = speed - dot(speed, transform->v_forward) * transform->v_forward;
    acceleration = acceleration - dot(acceleration, transform->v_forward) * transform->v_forward;
}
void PlayerController::StopRightMovement()
{
    speed = speed - dot(speed, transform->v_right) * transform->v_right;
    acceleration = acceleration - dot(acceleration, transform->v_right) * transform->v_right;
}

void PlayerController::StopUpMovement()
{
    speed = speed - dot(speed, transform->v_up) * transform->v_up;
    acceleration = acceleration - dot(acceleration, transform->v_up) * transform->v_up;
}

void PlayerController::Update(float deltaTime)
{
    if (!transform)
    {
        transform = parent->GetComponent<Transform>();
    }

    if (isPositionLocked)   return;
    
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

    vec3 localSpeed = transform->v_forward * speed.z
     + transform->v_right * speed.x
    + transform->v_up * speed.y;
    transform->v_position = transform->v_position + localSpeed * deltaTime;
}

void PlayerController::Shoot()
{
    std::shared_ptr<Projectile> projectile = std::make_shared<Projectile>();
    projectile->Initialize();
    World::GetInstance()->AddEntity(projectile);
}

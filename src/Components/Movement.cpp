#include "Movement.h"

#include "Transform.h"
#include "Entities/Entity.h"

void Movement::Update(float deltaTime)
{
    if (!transform)
    {
        transform = parent->GetComponent<Transform>();
    }

    if (length(acceleration) > maxAcceleration)
    {
        acceleration = normalize(acceleration) * maxAcceleration;
    }

    speed = speed + acceleration * deltaTime;
    if (length(speed) > maxSpeed)
    {
        speed = normalize(speed) * maxSpeed;
    }
    transform->position = transform->position + speed * deltaTime;
}

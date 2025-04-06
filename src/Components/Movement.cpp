#include "Movement.h"

#include <Utils/Debugger.h>
#include <Utils/Parser.h>

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
    if (isAffectedByGravity)
    {
        speed.y -= 9.8f * deltaTime;
    }
    if (length(speed) > maxSpeed)
    {
        speed = normalize(speed) * maxSpeed;
    }

    if (usesPhysics && length(impactNormal) > 0.0f)
    {
        float dotProduct = dot(speed, impactNormal);
        if (dotProduct < 0.0f)
        {
            speed = speed - dotProduct * impactNormal;
        }
    }
    transform->position = transform->position + speed * deltaTime;
    impactNormal = vec3(0.0f);
}

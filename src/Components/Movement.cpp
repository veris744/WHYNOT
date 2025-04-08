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
        for (int i = 0; i < 3; i++)
        {
            vec3 frag = vec3(0);
            if (i == 0) frag = vec3(impactNormal.x,0,0);
            if (i == 1) frag = vec3(0,impactNormal.y,0);
            if (i == 2) frag = vec3(0,0,impactNormal.z);

            float dotProduct = dot(speed, frag);
            if (dotProduct < 0.0f)
            {
                speed = speed - dotProduct * frag;
            }
        }
    }

    transform->position = transform->position + speed * deltaTime;
    impactNormal = vec3(0.0f);
}

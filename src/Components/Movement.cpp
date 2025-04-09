#include "Movement.h"

#include <Utils/Debugger.h>

#include "Transform.h"
#include "Entities/Entity.h"

void Movement::Update(float deltaTime)
{
    if (!transform) {
        transform = parent->GetComponent<Transform>();
    }

    if (isAffectedByGravity) {
        AddForce(vec3(0.0f, -15.0f * mass, 0.0f));
    }

    acceleration = accumulatedForce / mass;
    ResetForces();

    if (length(acceleration) > maxAcceleration) {
        acceleration = normalize(acceleration) * maxAcceleration;
    }

    speed += acceleration * deltaTime;
    if (length(speed) > maxSpeed) {
        speed = normalize(speed) * maxSpeed;
    }

    // Handle collisions
    if (usesPhysics && collisionNormals.size() > 0) {
        for (const vec3& normal : collisionNormals) {
            float dotProduct = dot(speed, normal);
            if (dotProduct < 0.0f) {
                speed -= dotProduct * normal;
            }
        }
        collisionNormals.clear();
    }
    transform->position += speed * deltaTime;
}

void Movement::AddForce(vec3 force)
{
    accumulatedForce += force;
}

void Movement::ResetForces()
{
    accumulatedForce = vec3(0.0f);
}

void Movement::AddImpulse(vec3 impulse)
{
    speed += impulse / mass;
}

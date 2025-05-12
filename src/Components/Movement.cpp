#include "Movement.h"

#include <Utils/Debugger.h>

#include "Transform.h"
#include "Entities/Entity.h"

void Movement::Update(float deltaTime)
{
    if (!transform) {
        transform = parent->GetComponent<Transform>();
    }

    if (physicsProperties.hasGravity) {
        AddForce(vec3(0.0f, -15.0f * physicsProperties.mass, 0.0f));
    }

    acceleration = accumulatedForce / physicsProperties.mass;
    ResetForces();

    if (length(acceleration) > maxAcceleration) {
        acceleration = normalize(acceleration) * maxAcceleration;
    }

    speed += acceleration * deltaTime;

    // Handle collisions
    if (usesPhysics && collisionNormals.size() > 0) {
    for (const vec3& normal : collisionNormals) {
        float dotProduct = dot(speed, normal);

        // Only process if moving toward the surface
        if (dotProduct <= 0.0f) {
            // Calculate bounce (reflect velocity and apply bounciness coefficient)
            speed -= (1.0f + physicsProperties.bounciness) * dotProduct * normal;

            // Apply friction along the surface (perpendicular to normal)
            vec3 tangent = speed - dot(speed, normal) * normal;
            if (length(tangent) > 0.0f) {
                speed -= tangent * physicsProperties.friction * deltaTime;
            }

            // Optional: Minimal velocity cutoff to prevent micro-bouncing
            if (length(speed) < 0.1f) {
                speed = vec3(0.0f);
            }
        }
    }
    collisionNormals.clear();
}

    float speedTotal = length(speed);
    if (speedTotal > maxSpeed) {
        speed = normalize(speed) * maxSpeed;
    }

    if (speedTotal < STOP_THRESHOLD)
    {
        speed = vec3(0.f);
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
    speed += impulse / physicsProperties.mass;
}

#include "Movement.h"

#include <Utils/Debugger.h>

#include "Transform.h"
#include "Entities/Entity.h"


void Movement::Update(float deltaTime)
{
    if (!transform) {
        transform = parent->GetComponent<Transform>();
    }
    if (!physics_material)
    {
        physics_material = parent->GetPhysicsMaterial();
    }

    if (physics_material->hasGravity) {
        AddForce(glm::vec3(0.0f, -15.0f * physics_material->mass, 0.0f));
    }

    acceleration = accumulatedForce / physics_material->mass;
    ResetForces();

    if (length(acceleration) > maxAcceleration) {
        acceleration = normalize(acceleration) * maxAcceleration;
    }

    speed += acceleration * deltaTime;

    // Handle collisions
    HandleCollisions(deltaTime);
    // if (usesPhysics && collisions.size() > 0) {
    //     for (const Hit& hit : collisions) {
    //         float dotProduct = dot(speed, hit.normal);
    //
    //         // Only process if moving toward the surface
    //         if (dotProduct <= 0.0f) {
    //             // Calculate bounce (reflect velocity and apply bounciness coefficient)
    //             speed -= (1.0f + physics_material->bounciness) * dotProduct * hit.normal;
    //
    //             // Apply friction along the surface (perpendicular to normal)
    //             glm::vec3 tangent = speed - dot(speed, hit.normal) * hit.normal;
    //             if (length(tangent) > 0.0f) {
    //                 speed -= tangent * physics_material->friction * deltaTime;
    //             }
    //
    //             // Optional: Minimal velocity cutoff to prevent micro-bouncing
    //             if (length(speed) < STOP_THRESHOLD) {
    //             speed = glm::vec3(0.0f);
    //             }
    //         }
    //     }
    //     collisions.clear();
    // }

    float speedTotal = length(speed);
    if (speedTotal > maxSpeed) {
        speed = normalize(speed) * maxSpeed;
    }

    if (speedTotal < STOP_THRESHOLD)
    {
        speed = glm::vec3(0.f);
    }
    transform->position += speed * deltaTime;
}

void Movement::HandleCollisions(float deltaTime) {
    if (usesPhysics && collisions.size() > 0) {
        for (const Hit& hit : collisions) {
            if (!hit.otherEntity) continue;

            Movement* otherMovement = hit.otherEntity->GetComponent<Movement>();
            PhysicsMaterial* otherMat = hit.otherEntity->GetPhysicsMaterial();
            if (!otherMat) continue;

            // Calculate inverse masses (more stable for static objects)
            float invMassA = 1.0f / physics_material->mass;
            float invMassB = otherMat->mass > 0 ? 1.0f / otherMat->mass : 0.0f;
            float totalInvMass = invMassA + invMassB;

            // Calculate relative velocity
            glm::vec3 otherVel = otherMovement ? otherMovement->speed : glm::vec3(0);
            glm::vec3 relativeVel = speed - otherVel;
            float velAlongNormal = dot(relativeVel, hit.normal);

            if (velAlongNormal <= 0.0f) {
                // Calculate restitution (properly handle zero bounciness)
                float restitution = (physics_material->bounciness + otherMat->bounciness) * 0.5f;
                restitution = std::clamp(restitution, 0.0f, 1.0f);  // Ensure 0-1 range

                // Calculate impulse
                float impulse = -(1.0f + restitution) * velAlongNormal / totalInvMass;

                // Apply impulses
                speed += hit.normal * (impulse * invMassA);
                if (otherMovement && otherMovement->usesPhysics) {
                    otherMovement->speed -= hit.normal * (impulse * invMassB);
                }

                // Position correction with energy conservation
                const float penetration = std::max(-velAlongNormal * deltaTime - 0.001f, 0.0f);
                const float correctionFactor = 0.4f * penetration;  // More stable factor
                glm::vec3 correction = correctionFactor * hit.normal / totalInvMass;

                transform->position += correction * invMassA;
                if (otherMovement) {
                    otherMovement->transform->position -= correction * invMassB;
                }

                // Velocity threshold to stop bouncing
                if (length(speed) < 0.05f && dot(speed, hit.normal) > -0.1f) {
                    speed = glm::vec3(0.0f);
                }

                // Improved friction with energy loss
                glm::vec3 tangentVel = relativeVel - hit.normal * velAlongNormal;
                if (length(tangentVel) > 0.01f) {
                    float frictionCoeff = (physics_material->friction + otherMat->friction) * 0.5f;
                    glm::vec3 frictionImpulse = -normalize(tangentVel) * frictionCoeff * deltaTime * 9.8f;

                    speed += frictionImpulse * invMassA;
                    if (otherMovement && otherMovement->usesPhysics) {
                        otherMovement->speed -= frictionImpulse * invMassB;
                    }
                }
            }
        }
        collisions.clear();
    }
}

void Movement::AddForce(glm::vec3 force)
{
    accumulatedForce += force;
}

void Movement::ResetForces()
{
    accumulatedForce = glm::vec3(0.0f);
}

void Movement::AddImpulse(glm::vec3 impulse)
{
    speed += impulse / physics_material->mass;
}

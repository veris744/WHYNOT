#include "Transform.h"
#include "Utils/Debugger.h"
#include "Utils/Parser.h"

glm::vec3 Transform::worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::mat4 Transform::GetModelMatrix(glm::vec3 _relativePosition, glm::vec3 relativeRotation, glm::vec3 relativeScale) const
{
    glm::quat entityRotationQuat = rotation.quat;
    glm::quat relativeRotationQuat = glm::quat(glm::radians(relativeRotation));

    // Scale the model's position relative to the entity
    glm::vec3 scaled_model_position = scale * _relativePosition;

    // Rotate the scaled position into world space
    glm::vec3 rotated_model_position = entityRotationQuat * (relativeRotationQuat * scaled_model_position);

    // Compute the final position in world space
    glm::vec3 final_position = position + rotated_model_position;

    // Compute the final rotation (concatenation of quaternions)
    glm::quat final_rotation = entityRotationQuat * relativeRotationQuat;

    // Compute the final scale
    glm::vec3 final_scale = scale * relativeScale;

    // Construct the transformation matrix (T * R * S)
    glm::mat4 transform = translate(glm::mat4(1.0f), final_position) *
                          mat4_cast(final_rotation) *
                          glm::scale(glm::mat4(1.0f), final_scale);
    
    return  transform;
}

void Transform::Update(float deltaTime)
{
    if (old_rot != rotation.vector())
    {
        rotation.UpdateQuaternion();
    }
}

void Transform::SetRotation(glm::vec3 _rotation)
{
    float pitch = _rotation.x;
    float yaw = _rotation.y;
    float roll = _rotation.z;

    rotation.SetRotation(pitch, yaw, roll);

    // Calculate forward vector
    glm::vec3 direction;
    direction.z = -cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.x = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward = normalize(direction);

    // Compute the right vector (ensure right-handed system)
    right = normalize(cross(forward, worldUp));

    // Compute the up vector
    up = normalize(cross(right, forward));
}

void Transform::SetRotation(float pitch, float yaw, float roll)
{
    rotation.SetRotation(pitch, yaw, roll);
    
    // Calculate forward vector
    glm::vec3 direction;
    direction.z = -cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.x = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward = normalize(direction);
    
    // Compute the right vector (ensure right-handed system)
    right = normalize(cross(forward, worldUp));

    // Compute the up vector
    up = normalize(cross(right, forward));
}

void Transform::Initialize()
{
    Component::Initialize();
    SetRotation(rotation.pitch, rotation.yaw, rotation.roll);
}

void Transform::RenderDebug()
{
    Debugger::DrawLineDebug(position, position + 2.f * forward, glm::vec3(1.0f, 0.0f, 0.0f));
    Debugger::DrawLineDebug(position, position + 2.f * up, glm::vec3(0.0f, 1.0f, 0.0f));
    Debugger::DrawLineDebug(position, position + 2.f * right, glm::vec3(0.0f, 0.0f, 1.0f));
}

void Transform::LookAt(glm::vec3 target)
{
    // Update direction vectors
    forward = normalize(target - position);
    right = normalize(cross(worldUp, forward));
    up = cross(forward, right);

    // Update rotation (yaw, pitch, roll) if needed
    float yaw = glm::degrees(atan2(forward.x, forward.z));
    float pitch = glm::degrees(atan2(-forward.y, sqrt(forward.x * forward.x + forward.z * forward.z)));
    
    rotation.SetRotation(pitch, yaw, rotation.roll);
}

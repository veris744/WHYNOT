#include "Transform.h"
#include "Utils/Debugger.h"
#include "Utils/Parser.h"

vec3 Transform::worldUp = vec3(0.0f, 1.0f, 0.0f);

mat4 Transform::GetModelMatrix(vec3 _relativePosition, vec3 relativeRotation, vec3 relativeScale) const
{
    quat entityRotationQuat = rotation.quat;
    quat relativeRotationQuat = quat(radians(relativeRotation));

    // Scale the model's position relative to the entity
    vec3 scaled_model_position = scale * _relativePosition;

    // Rotate the scaled position into world space
    vec3 rotated_model_position = entityRotationQuat * (relativeRotationQuat * scaled_model_position);

    // Compute the final position in world space
    vec3 final_position = position + rotated_model_position;

    // Compute the final rotation (concatenation of quaternions)
    quat final_rotation = entityRotationQuat * relativeRotationQuat;

    // Compute the final scale
    vec3 final_scale = scale * relativeScale;

    // Construct the transformation matrix (T * R * S)
    mat4 transform = translate(mat4(1.0f), final_position) *
                          mat4_cast(final_rotation) *
                          glm::scale(mat4(1.0f), final_scale);
    
    return  transform;
}

void Transform::Update(float deltaTime)
{
    if (old_rot != rotation.vector())
    {
        rotation.UpdateQuaternion();
    }
}

void Transform::SetRotation(vec3 _rotation)
{
    float pitch = _rotation.x;
    float yaw = _rotation.y;
    float roll = _rotation.z;

    rotation.SetRotation(pitch, yaw, roll);

    // Calculate forward vector
    vec3 direction;
    direction.z = -cos(radians(yaw)) * cos(radians(pitch));
    direction.y = sin(radians(pitch));
    direction.x = sin(radians(yaw)) * cos(radians(pitch));
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
    vec3 direction;
    direction.z = -cos(radians(yaw)) * cos(radians(pitch));
    direction.y = sin(radians(pitch));
    direction.x = sin(radians(yaw)) * cos(radians(pitch));
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
    Debugger::DrawLineDebug(position, position + 2.f * forward, vec3(1.0f, 0.0f, 0.0f));
    Debugger::DrawLineDebug(position, position + 2.f * up, vec3(0.0f, 1.0f, 0.0f));
    Debugger::DrawLineDebug(position, position + 2.f * right, vec3(0.0f, 0.0f, 1.0f));
}

void Transform::LookAt(vec3 target)
{
    // Update direction vectors
    forward = normalize(target - position);
    right = normalize(cross(worldUp, forward));
    up = cross(forward, right);

    // Update rotation (yaw, pitch, roll) if needed
    float yaw = degrees(atan2(forward.x, forward.z));
    float pitch = degrees(atan2(-forward.y, sqrt(forward.x * forward.x + forward.z * forward.z)));
    
    rotation.SetRotation(pitch, yaw, rotation.roll);
}

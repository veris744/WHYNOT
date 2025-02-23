#include "Transform.h"
#include "includes.h"
#include "Utils/Parser.h"

vec3 Transform::worldUp = vec3(0.0f, 1.0f, 0.0f);

mat4 Transform::GetModelMatrix(vec3 _relativePosition, vec3 relativeRotation, vec3 relativeScale) const
{
    glm::quat entityRotationQuat = rotation.quat;
    glm::quat relativeRotationQuat = glm::quat(radians(relativeRotation));

    // Scale the model's position relative to the entity
    vec3 scaled_model_position = scale * _relativePosition;

    // Rotate the scaled position into world space
    vec3 rotated_model_position = entityRotationQuat * (relativeRotationQuat * scaled_model_position);

    // Compute the final position in world space
    vec3 final_position = position + rotated_model_position;

    // Compute the final rotation (concatenation of quaternions)
    glm::quat final_rotation = entityRotationQuat * relativeRotationQuat;

    // Compute the final scale
    vec3 final_scale = scale * relativeScale;

    // Construct the transformation matrix (T * R * S)
    mat4 transform = translate(mat4(1.0f), final_position) *
                          mat4_cast(final_rotation) *
                          glm::scale(mat4(1.0f), final_scale);


    // Logger::Log(LogLevel::Warning, Parser::Parse(rotation.vector()));
    // Logger::Log(LogLevel::Info, Parser::Parse(rotation.quat));
    // glm::vec3 eulerAngles = glm::degrees(glm::eulerAngles(rotation.quat));
    // Logger::Log(LogLevel::Error, Parser::Parse(eulerAngles));
    // Logger::Log(LogLevel::Info, Parser::Parse(entityRotation));
    // Logger::Log(LogLevel::Info, Parser::Parse(relativeRotation));
    // Logger::Log(LogLevel::Warning, Parser::Parse(final_rotation));
    
    return  transform;
}

void Transform::Update(float deltaTime)
{
    
}

void Transform::SetRotation(float pitch, float yaw, float roll)
{
    rotation.SetRotation(pitch, yaw, roll);
    
    // Calculate forward vector
    vec3 direction;
    direction.z = cos(radians(yaw)) * cos(radians(pitch));
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
    debugEnabled = true;
}

void Transform::RenderDebug()
{
    //Debugger::DrawLineDebug(position, position + 5.f * forward + 5.f * up, vec3(1.0f, 0.0f, 0.0f));
    //Debugger::DrawLineDebug(position, position + 10.f * up, vec3(0.0f, 1.0f, 0.0f));
    //Debugger::DrawLineDebug(position, position + 10.f * right, vec3(0.0f, 0.0f, 1.0f));
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

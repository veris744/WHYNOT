#include "Transform.h"



mat4 Transform::GetModelMatrix(vec3 _relativePosition, bool _invert) const
{
    mat4 trans = mat4(1.0f);
    if (_invert)
    {
        trans = mat4(
            1,  0,  0,  0,
            0, -1,  0,  0,
            0,  0,  1,  0,
            0,  0,  0,  1);
    }
    vec3 trueDiff = _relativePosition.x * right + _relativePosition.y * forward + _relativePosition.z * up;
    trans = translate(trans, position + trueDiff);
    trans = rotate(trans, glm::radians(rotation.yaw), up);
    trans = rotate(trans, glm::radians(rotation.pitch), right);
    trans = rotate(trans, glm::radians(rotation.roll), forward);
    trans = glm::scale(trans, scale);
    
    return trans;
}

void Transform::Update(float deltaTime)
{
    
}

void Transform::SetRotation(float pitch, float yaw, float roll)
{
    rotation.SetRotation(pitch, yaw, roll);
    
    vec3 direction;
    direction.z = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.x = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward = normalize(direction);
    
    vec3 worldUp = vec3(0.0f, 1.0f, 0.0f); 
    right = normalize(cross(worldUp, forward));
    
    up = cross(forward, right);
}

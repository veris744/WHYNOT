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
    vec3 trueDiff = _relativePosition.x * v_right + _relativePosition.y * v_forward + _relativePosition.z * v_up;
    trans = translate(trans, v_position + trueDiff);
    trans = rotate(trans, glm::radians(v_rotation.yaw), v_up);
    trans = rotate(trans, glm::radians(v_rotation.pitch), v_right);
    trans = rotate(trans, glm::radians(v_rotation.roll), v_forward);
    trans = scale(trans, v_scale);
    
    return trans;
}

void Transform::Update(float deltaTime)
{
    
}

void Transform::SetRotation(float pitch, float yaw, float roll)
{
    v_rotation.SetRotation(pitch, yaw, roll);
    
    vec3 direction;
    direction.z = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.x = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    v_forward = normalize(direction);
    
    vec3 worldUp = vec3(0.0f, 1.0f, 0.0f); 
    v_right = normalize(cross(worldUp, v_forward));
    
    v_up = cross(v_forward, v_right);
}

#pragma once
#include <includes.h>

#include "Reflection/Reflection.h"

struct Rotation
{
    float pitch = 0.0f;
    float yaw = 0.0f;
    float roll = 0.0f;

    void SetRotation(float _pitch, float _yaw, float _roll)
    {
        pitch = _pitch;
        yaw = _yaw;
        roll = _roll;
    }

    void SetRotation(vec3 _rotation)
    {
        pitch = _rotation.x;
        yaw = _rotation.y;
        roll = _rotation.z;
    }
    
    void SetRotationFromDirection(vec3 _forward, vec3 _up)
    {
        vec3 fwd = normalize(_forward);
        vec3 upVec = normalize(_up);
        yaw = glm::degrees(atan2(fwd.x, fwd.z));
        pitch = glm::degrees(asin(fwd.y));
        vec3 right = normalize(cross(upVec, fwd));
        roll = glm::degrees(atan2(right.y, upVec.y));
    }    
    
};
REGISTER_CLASS(Rotation, {
    REGISTER_MEMBER(Rotation, pitch),
    REGISTER_MEMBER(Rotation, yaw),
    REGISTER_MEMBER(Rotation, roll)
});
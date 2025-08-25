#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Reflection/ReflectedObject.h"
#include "Reflection/Reflection.h"

struct Rotation : ReflectedObject
{
    float pitch = 0.0f;
    float yaw = 0.0f;
    float roll = 0.0f;

    glm::quat quat = glm::quat();

    void SetRotation(float _pitch, float _yaw, float _roll)
    {
        pitch = _pitch;
        yaw = _yaw;
        roll = _roll;
        quat = glm::quat(radians(glm::vec3(pitch, yaw, roll)));
    }

    void SetRotation(glm::vec3 _rotation)
    {
        pitch = _rotation.x;
        yaw = _rotation.y;
        roll = _rotation.z;
        quat = glm::quat(glm::radians(glm::vec3(pitch, yaw, roll)));
    }
    
    void SetRotation(glm::quat _quat)
    {
        glm::vec3 vec = glm::degrees(glm::eulerAngles(quat));
        pitch = vec.x;
        yaw = vec.y;
        roll = vec.z;
    }
    
    void SetRotationFromDirection(glm::vec3 _forward, glm::vec3 _up)
    {
        glm::vec3 fwd = normalize(_forward);
        glm::vec3 upVec = normalize(_up);
        yaw = glm::degrees(atan2(fwd.x, fwd.z));
        pitch = glm::degrees(asin(fwd.y));
        glm::vec3 right = normalize(cross(upVec, fwd));
        roll = glm::degrees(atan2(right.y, upVec.y));
        
        quat = glm::quat(radians(glm::vec3(pitch, yaw, roll)));
    }

    void UpdateQuaternion()
    {
        quat = glm::quat(glm::radians(glm::vec3(pitch, yaw, roll)));
    }

    glm::vec3 vector() const { return glm::vec3(pitch, yaw, roll); };
    
};
REGISTER_CLASS(Rotation, {
    REGISTER_MEMBER(Rotation, pitch, MemberProperty::Viewable | MemberProperty::Editable | MemberProperty::Serializable),
    REGISTER_MEMBER(Rotation, yaw, MemberProperty::Viewable | MemberProperty::Editable | MemberProperty::Serializable),
    REGISTER_MEMBER(Rotation, roll, MemberProperty::Viewable | MemberProperty::Editable | MemberProperty::Serializable)
});
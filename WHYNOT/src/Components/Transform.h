#pragma once
#include <includes.h>

#include "Component.h"
#include "Reflection/Reflection.h"
#include "Utils/Rotation.h"

class Transform : public Component
{
    
public:
    Transform(vec3 _pos = vec3(0))
        : position(_pos),
        scale(vec3(1, 1, 1)),
        forward(vec3(0, 0, 1)),
        up(vec3(0, 1, 0)),
        right(vec3(1, 0, 0))
    {
        name = "TRANSFORM";
        rotation.SetRotationFromDirection(forward, up);
    }

    vec3 position;
    vec3 scale;
    Rotation rotation;

    vec3 forward;
    vec3 up;
    vec3 right;


    mat4 GetModelMatrix(vec3 _relativePosition = vec3(0,0,0), bool _invert = false) const;
    void Update(float deltaTime) override;
    void SetRotation(float pitch, float yaw, float roll);
};
REGISTER_CLASS(Transform, 
    REGISTER_MEMBER(Transform, position),
    REGISTER_MEMBER(Transform, scale),
    REGISTER_MEMBER(Transform, rotation)
);
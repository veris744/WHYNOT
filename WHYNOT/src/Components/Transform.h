#pragma once
#include <includes.h>

#include "Component.h"
#include "Utils/Rotation.h"

class Transform : public Component
{
    
public:
    Transform(vec3 _pos = vec3(0))
        : v_position(_pos),
        v_scale(vec3(1, 1, 1)),
        v_forward(vec3(0, 0, 1)),
        v_up(vec3(0, 1, 0)),
        v_right(vec3(1, 0, 0))
        
    {
        name = "TRANSFORM";
        v_rotation.SetRotationFromDirection(v_forward, v_up);
    }

    vec3 v_position;
    vec3 v_scale;
    Rotation v_rotation;

    vec3 v_forward;
    vec3 v_up;
    vec3 v_right;


    mat4 GetModelMatrix(vec3 _relativePosition = vec3(0,0,0), bool _invert = false) const;
    void Update(float deltaTime) override;
    void SetRotation(float pitch, float yaw, float roll);
    
};

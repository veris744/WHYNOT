#pragma once
#include <Utils/Rotation.h>

#include "Component.h"
#include "Reflection/Reflection.h"

class Transform : public Component
{
    static vec3 worldUp;

    vec3 old_rot = vec3(0, 0, 0);
    
public:
    Transform(vec3 _pos = vec3(0))
        : position(_pos),
        scale(vec3(1, 1, 1)),
        forward(vec3(0, 0, 1)),
        up(vec3(0, 1, 0)),
        right(vec3(1, 0, 0))
    {
        name = "TRANSFORM";
    }

    vec3 position;
    vec3 scale;
    Rotation rotation;

    vec3 forward;
    vec3 up;
    vec3 right;


    mat4 GetModelMatrix(vec3 _relativePosition = vec3(0,0,0),
        vec3 relativeRotation = {0,0,0},
        vec3 relativeScale = {0,0,0}) const;
    void Update(float deltaTime) override;
    void SetRotation(float pitch, float yaw, float roll);
    void Initialize() override;
    void RenderDebug() override;

    void LookAt(vec3 target);
};
REGISTER_CLASS(Transform, 
    REGISTER_MEMBER(Transform, position, MemberProperty::Viewable | MemberProperty::Editable),
    REGISTER_MEMBER(Transform, scale, MemberProperty::Viewable | MemberProperty::Editable),
    REGISTER_MEMBER(Transform, rotation, MemberProperty::Viewable)
);
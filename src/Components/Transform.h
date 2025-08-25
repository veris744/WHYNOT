#pragma once
#include <Utils/Rotation.h>

#include "Component.h"
#include "Reflection/Reflection.h"

class Transform : public Component
{
    static glm::vec3 worldUp;

    glm::vec3 old_rot = glm::vec3(0, 0, 0);
    
public:
    Transform(glm::vec3 _pos = glm::vec3(0))
        : position(_pos),
        scale(glm::vec3(1, 1, 1)),
        forward(glm::vec3(0, 0, 1)),
        up(glm::vec3(0, 1, 0)),
        right(glm::vec3(1, 0, 0))
    {
        name = "TRANSFORM";
        isSkippedWhenPause = false;
    }

    //glm::vec3 collision_pos = glm::vec3(0, 0, 0);

    glm::vec3 position;
    glm::vec3 scale;
    Rotation rotation;

    glm::vec3 forward;
    glm::vec3 up;
    glm::vec3 right;


    glm::mat4 GetModelMatrix(glm::vec3 _relativePosition = glm::vec3(0,0,0),
        glm::vec3 relativeRotation = {0,0,0},
        glm::vec3 relativeScale = {0,0,0}) const;
    void Update(float deltaTime) override;
    void SetRotation(glm::vec3 _rotation);
    void SetRotation(float pitch, float yaw, float roll);
    void Initialize() override;
    void RenderDebug() override;

    void LookAt(glm::vec3 target);
};
REGISTER_CLASS(Transform,
    REGISTER_MEMBER(Transform, name, MemberProperty::Serializable),
    REGISTER_MEMBER(Transform, position, MemberProperty::Viewable | MemberProperty::Editable | MemberProperty::Serializable),
    REGISTER_MEMBER(Transform, scale, MemberProperty::Viewable | MemberProperty::Editable | MemberProperty::Serializable),
    REGISTER_MEMBER(Transform, rotation, MemberProperty::Viewable | MemberProperty::Serializable)
);
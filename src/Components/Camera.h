#pragma once
#include "Component.h"
#include "Reflection/Reflection.h"

class Transform;

class Camera : public Component
{

    glm::vec3 cameraForward;

    Transform* transform = nullptr;
    
public:
    Camera()
    {
        name = "CAMERA";
    }

    float near = 0.1f;
    float far = 100.f;
    float fov = 45.0f;
    
    glm::mat4 GetProjectionMatrix() const;
    glm::mat4 GetProjectionMatrix2D() const;
    glm::mat4 GetViewMatrix();
    glm::vec3 GetViewPos();

    void Update(float deltaTime) override;
    
};
REGISTER_CLASS(Camera,
    REGISTER_MEMBER(Camera, name, MemberProperty::Serializable),
    REGISTER_MEMBER(Camera, near, MemberProperty::Viewable),
    REGISTER_MEMBER(Camera, far, MemberProperty::Viewable),
    REGISTER_MEMBER(Camera, fov, MemberProperty::Viewable)
);
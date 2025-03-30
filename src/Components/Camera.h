#pragma once
#include "Component.h"
#include "Reflection/Reflection.h"

class Transform;

class Camera : public Component
{
    float near = 0.1f;
    float far = 100.f;
    float fov = 45.0f;

    vec3 cameraForward;

    Transform* transform = nullptr;
    
public:
    Camera()
    {
        name = "CAMERA";
    }
    
    mat4 GetProjectionMatrix() const;
    mat4 GetProjectionMatrix2D() const;
    mat4 GetViewMatrix();
    vec3 GetViewPos();

    void Update(float deltaTime) override;
    
};
REGISTER_CLASS(Camera,
    REGISTER_MEMBER(Camera, name, MemberProperty::Serializable)
);
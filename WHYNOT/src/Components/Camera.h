#pragma once
#include <includes.h>

#include "Component.h"
#include "Reflection/Reflection.h"

class Camera : public Component
{
    float near = 0.1f;
    float far = 100.f;
    float fov = 45.0f;
    
public:
    Camera()
    {
        name = "CAMERA";
    }
    
    mat4 GetProjectionMatrix() const;
    mat4 GetProjectionMatrix2D() const;
    mat4 GetViewMatrix() const;
    vec3 GetViewPos() const;

    void Update(float deltaTime) override;
    
};
REGISTER_CLASS(Camera)
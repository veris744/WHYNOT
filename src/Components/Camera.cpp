#include "Camera.h"

#include "Transform.h"
#include "Entities/Entity.h"
#include "Managers/Helper.h"

mat4 Camera::GetProjectionMatrix() const
{
    float aspect = Helper::windowWidth / Helper::windowHeight;
    mat4 proj = glm::perspective(radians(fov), aspect, near, far);
    return proj;
}

mat4 Camera::GetProjectionMatrix2D() const
{
    return glm::ortho(0.f, Helper::windowWidth, Helper::windowHeight, 0.f, -1.f, 1.f);
}

mat4 Camera::GetViewMatrix()
{
    if (!transform)
    {
        transform = parent->GetComponent<Transform>();
    }
    cameraForward = transform->forward;
    mat4 view = lookAt(transform->position, transform->position + cameraForward, transform->up);
    return view;
    
}

vec3 Camera::GetViewPos()
{
    if (!transform)
    {
        transform = parent->GetComponent<Transform>();
    }
    
    return transform->position;
}

void Camera::Update(float deltaTime)
{
    
}

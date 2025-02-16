#include "Camera.h"

#include "Transform.h"
#include "Entities/Entity.h"
#include "Managers/Helper.h"

mat4 Camera::GetProjectionMatrix() const
{
    float aspect = Helper::windowWidth / Helper::windowHeight;
    mat4 proj = glm::perspective(glm::radians(fov), aspect, near, far);
    return proj;
}

mat4 Camera::GetProjectionMatrix2D() const
{
    return glm::ortho(0.f, Helper::windowWidth, Helper::windowHeight, 0.f, -1.f, 1.f);
}

mat4 Camera::GetViewMatrix() const
{
    std::shared_ptr<Transform> transform = parent->GetComponent<Transform>();
    if (!transform)
    {
        Logger::Log<Camera>(LogLevel::Error, "No Camera Transform");
        return mat4();
    }
    
    mat4 view = lookAt(transform->position, transform->position + transform->forward, transform->up);
    return view;
    
}

vec3 Camera::GetViewPos() const
{
    std::shared_ptr<Transform> transform = parent->GetComponent<Transform>();
    if (!transform)
    {
        Logger::Log<Camera>(LogLevel::Error, "No Camera Transform");
        return vec3(0, 0, 0);
    }
    
    return transform->position;
}

void Camera::Update(float deltaTime)
{
    
}

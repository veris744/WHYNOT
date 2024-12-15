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
        Logger::Log<Camera>(LogLevel::FatalError, "No Camera Transform");
    }
    
    mat4 view = lookAt(transform->v_position, transform->v_position + transform->v_forward, transform->v_up);
    return view;
    
}

vec3 Camera::GetViewPos() const
{
    std::shared_ptr<Transform> transform = parent->GetComponent<Transform>();
    if (!transform)
    {
        Logger::Log<Camera>(LogLevel::FatalError, "No Camera Transform");
    }
    
    return transform->v_position;
}

void Camera::Update(float deltaTime)
{
    
}

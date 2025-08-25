#include "Camera.h"

#include "Transform.h"
#include "Entities/Entity.h"
#include "Managers/Helper.h"

glm::mat4 Camera::GetProjectionMatrix() const
{
    float aspect = Helper::windowWidth / Helper::windowHeight;
    glm::mat4 proj = glm::perspective(glm::radians(fov), aspect, near, far);
    return proj;
}

glm::mat4 Camera::GetProjectionMatrix2D() const
{
    return glm::ortho(0.f, Helper::windowWidth, Helper::windowHeight, 0.f, -1.f, 1.f);
}

glm::mat4 Camera::GetViewMatrix()
{
    if (!transform)
    {
        transform = parent->GetComponent<Transform>();
    }
    cameraForward = transform->forward;
    glm::mat4 view = lookAt(transform->position, transform->position + cameraForward, transform->up);
    return view;
    
}

glm::vec3 Camera::GetViewPos()
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

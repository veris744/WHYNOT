#include "InputManager.h"

#include "EventsBuffer.h"
#include "Managers/Helper.h"
#include "Managers/World.h"
#include "Components/Transform.h"



////////////////////////////////////////////////////////
/// INPUT MANAGER DEFINITIONS //////////////////////////
////////////////////////////////////////////////////////

InputManager* InputManager::instance = nullptr;
unordered_map<unsigned int, KeyStatus> InputManager::keysStatus;
std::unique_ptr<EventsBuffer> InputManager::eventsBuffer = std::make_unique<EventsBuffer>();


InputManager* InputManager::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new InputManager();
        InitKeys();
        glfwSetInputMode(Helper::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetCursorPosCallback(Helper::GetWindow(), MouseCallback);
        glfwSetKeyCallback(Helper::GetWindow(), KeyCallback);
        glfwSetMouseButtonCallback(Helper::GetWindow(), MouseButtonCallback);
        glfwSetScrollCallback(Helper::GetWindow(), ScrollCallback);
    }
    return instance;
}


void InputManager::Initialize()
{
    if (!cameraTransform)
    {
        cameraTransform = World::GetInstance()->GetCamera(World::GetInstance()->currentCameraName)->GetComponent<Transform>();
    }
}

void InputManager::InitKeys()
{
    for (const auto& actionPair : actions)
    {
        keysStatus[actionPair.second] = KeyStatus::RELEASED;
    }
}

////////////////////////////////////////////////////////
/// CALLBACKS //////////////////////////////////////////
////////////////////////////////////////////////////////

void InputManager::MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    InputEvent event = {EventType::MouseMove, -1, -1, xpos, ypos};
    eventsBuffer->AddEvent(event);
}

void InputManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    InputEvent event = {EventType::KeyPress, key, action, 0, 0, mods};
    if (action == GLFW_RELEASE)
    {
        event.type = EventType::KeyRelease;
    }
    // if (keysStatus[key] == KeyStatus::RELEASED)
    // {
    //     keysStatus[key] = KeyStatus::PRESSED;
    // }
    // else
    // {
    //     keysStatus[key] = KeyStatus::RELEASED;
    //     event.type = EventType::KeyRelease;
    // }
    eventsBuffer->AddEvent(event);
}

void InputManager::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    InputEvent event = {EventType::MouseButtonPress, button, action, 0, 0, mods};
    if (action == GLFW_RELEASE)
    {
        event.type = EventType::KeyRelease;
    }
    // if (keysStatus[button] == KeyStatus::RELEASED)
    // {
    //     keysStatus[button] = KeyStatus::PRESSED;
    // }
    // else
    // {
    //     keysStatus[button] = KeyStatus::RELEASED;
    //     event.type = EventType::MouseButtonRelease;
    // }
    eventsBuffer->AddEvent(event);
}

void InputManager::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    InputEvent event = {EventType::MouseScroll, -1, -1, xoffset, yoffset};
    eventsBuffer->AddEvent(event);
}


////////////////////////////////////////////////////////
/// PROCESSING /////////////////////////////////////////
////////////////////////////////////////////////////////

void InputManager::Update(float _deltaTime)
{
    deltaTime = _deltaTime;
    
    ProcessInput();
    MovementInput();
}


void InputManager::ProcessInput()
{
    while (!eventsBuffer->IsEmpty())
    {
        InputEvent event = eventsBuffer->PopEvent();
        switch (event.type)
        {
        case EventType::KeyPress:
            HandleKeyPress(event.key, event.mods);
            break;
            
        case EventType::KeyRelease:
            HandleKeyRelease(event.key, event.mods);
            break;
            
        case EventType::MouseButtonPress:
            HandleMouseButtonPress(event.key);
            break;
            
        case EventType::MouseButtonRelease:
            HandleMouseButtonRelease(event.key);
            break;
            
        case EventType::MouseMove:
            HandleMouseMove(event.x, event.y);
            break;
            
        case EventType::MouseScroll:
            HandleMouseScroll(event.x, event.y);
            break;
        }
    }
}

void InputManager::HandleKeyPress(int key, int mods)
{
    switch(key)
    {
        case GLFW_KEY_ESCAPE:
            ScapeInput();
            break;
        case GLFW_KEY_UP:
            movementInputPlaneValue.y = 1;
            break;
        case GLFW_KEY_DOWN:
            movementInputPlaneValue.y = -1;
            break;
        case GLFW_KEY_A:
            movementInputPlaneValue.x = 1;
            break;
        case GLFW_KEY_D:
            movementInputPlaneValue.x = -1;
            break;
        case GLFW_KEY_S:
            movementInputPlaneValue.z = -1;
            break;
        case GLFW_KEY_W:
            movementInputPlaneValue.z = 1;
            break;
        default: 
            break;
    }
}

void InputManager::HandleKeyRelease(int key, int mods)
{
    switch(key)
    {
    case GLFW_KEY_UP:
        movementInputPlaneValue.y = 0;
        break;
    case GLFW_KEY_DOWN:
        movementInputPlaneValue.y = 0;
        break;
    case GLFW_KEY_A:
        movementInputPlaneValue.x = 0;
        break;
    case GLFW_KEY_D:
        movementInputPlaneValue.x = 0;
        break;
    case GLFW_KEY_S:
        movementInputPlaneValue.z = 0;
        break;
    case GLFW_KEY_W:
        movementInputPlaneValue.z = 0;
        break;
    default: 
        break;
    }
}

void InputManager::HandleMouseButtonPress(int key)
{
    double xpos, ypos;
    glfwGetCursorPos(Helper::GetWindow(), &xpos, &ypos);
    OnClickDelegate.Execute(vec2(xpos, ypos));
}

void InputManager::HandleMouseButtonRelease(int key)
{
}

void InputManager::HandleMouseMove(double x, double y)
{
    vec2 pos = {x, y};
    if (firstMouse)
    {
        rotation = cameraTransform->v_rotation;
        lastMousePos.x = pos.x;
        lastMousePos.y = pos.y;
        firstMouse = false;
    }
  
    float xoffset = pos.x - lastMousePos.x;
    float yoffset = lastMousePos.y - pos.y; 
    lastMousePos.x = pos.x;
    lastMousePos.y = pos.y;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= -sensitivity;

    rotation.yaw -= xoffset;
    rotation.pitch -= yoffset;

    if(rotation.pitch > 89.0f)
        rotation.pitch = 89.0f;
    if(rotation.pitch < -89.0f)
        rotation.pitch = -89.0f;

    cameraTransform->SetRotation(rotation.pitch, rotation.yaw, rotation.roll);
}

void InputManager::HandleMouseScroll(double x, double y)
{
}

void InputManager::ScapeInput() const 
{
    glfwSetWindowShouldClose(Helper::GetWindow(), true);
}

void InputManager::MovementInput() const
{
    vec3 direction = cameraTransform->v_forward * movementInputPlaneValue.z
        + cameraTransform->v_right * movementInputPlaneValue.x
        + cameraTransform->v_up * movementInputPlaneValue.y;

    cameraTransform->v_position += direction * deltaTime;
}


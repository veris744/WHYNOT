#include "InputManager.h"

#include "EventsBuffer.h"
#include "Components/PlayerController.h"
#include "Managers/Helper.h"
#include "Managers/World.h"
#include "Components/Transform.h"
#include "Managers/Renderer.h"
#include "Minigame1/AliensLogic.h"
#include "Utils/Debugger.h"


////////////////////////////////////////////////////////
/// INPUT MANAGER DEFINITIONS //////////////////////////
////////////////////////////////////////////////////////

std::shared_ptr<InputManager> InputManager::instance = nullptr;
unordered_map<unsigned int, KeyStatus> InputManager::keysStatus;
std::unique_ptr<EventsBuffer> InputManager::eventsBuffer = std::make_unique<EventsBuffer>();
InputMode InputManager::inputMode = InputMode::UIOnly;
bool InputManager::isInputEnabled = false;

std::shared_ptr<InputManager> InputManager::GetInstance()
{
    if (instance == nullptr)
    {
        instance = std::make_shared<InputManager>(InputManager());
        InitKeys();
        glfwSetInputMode(Helper::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetCursorPosCallback(Helper::GetWindow(), MouseCallback);
        glfwSetKeyCallback(Helper::GetWindow(), KeyCallback);
        glfwSetMouseButtonCallback(Helper::GetWindow(), MouseButtonCallback);
        glfwSetScrollCallback(Helper::GetWindow(), ScrollCallback);
    }
    return instance;
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
    if (!isInputEnabled)  return;
    
    InputEvent event = {EventType::MouseMove, -1, -1, xpos, ypos};
    eventsBuffer->AddEvent(event);
}

void InputManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (!isInputEnabled)  return;
    
    InputEvent event = {EventType::KeyPress, key, action, 0, 0, mods};
    if (action == GLFW_RELEASE)
    {
        event.type = EventType::KeyRelease;
    }
    else
    {
        event.type = EventType::KeyPress;
    }
    eventsBuffer->AddEvent(event);
}

void InputManager::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (!isInputEnabled)
    {
        return;
    }
    
    InputEvent event = {EventType::MouseButtonPress, button, action, 0, 0, mods};
    if (action == GLFW_RELEASE)
    {
        event.type = EventType::MouseButtonRelease;
    }
    else
    {
        event.type = EventType::MouseButtonPress;
    }
    eventsBuffer->AddEvent(event);
}

void InputManager::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (!isInputEnabled)  return;
    
    InputEvent event = {EventType::MouseScroll, -1, -1, xoffset, yoffset};
    eventsBuffer->AddEvent(event);
}


////////////////////////////////////////////////////////
/// PROCESSING /////////////////////////////////////////
////////////////////////////////////////////////////////

void InputManager::Update(float _deltaTime)
{    
    if (!playerController)
    {
        playerController = World::GetInstance()->GetPlayer()->GetComponent<PlayerController>();
    }
    if (!playerTransform)
    {
        playerTransform = World::GetInstance()->GetPlayer()->GetComponent<Transform>();
    }
    ProcessInput();
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
        case GLFW_KEY_Q:
            ScapeInput();
        break;
        case GLFW_KEY_UP:
            playerController->SetInput(vec3(0,1,0));
            break;
        case GLFW_KEY_DOWN:
            playerController->SetInput(vec3(0,-1,0));
            break;
        case GLFW_KEY_A:
            playerController->SetInput(vec3(1,0,0));
            break;
        case GLFW_KEY_D:
            playerController->SetInput(vec3(-1,0,0));
            break;
        case GLFW_KEY_S:
            playerController->SetInput(vec3(0,0,1));
            break;
        case GLFW_KEY_W:
            playerController->SetInput(vec3(0,0,-1));
            break;
    case GLFW_KEY_1:
            Debugger::SetCollisionDebug(!Debugger::GetCollisionDebugEnabled());
        break;
        case GLFW_KEY_P:
            AliensLogic::GetInstance()->StopGame();
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
        playerController->SetInput(vec3(playerController->GetInput().x,0,playerController->GetInput().z));
        break;
    case GLFW_KEY_DOWN:
        playerController->SetInput(vec3(playerController->GetInput().x,0,playerController->GetInput().z));
        break;
    case GLFW_KEY_A:
        playerController->SetInput(vec3(0,playerController->GetInput().y,playerController->GetInput().z));
        break;
    case GLFW_KEY_D:
        playerController->SetInput(vec3(0, playerController->GetInput().y,playerController->GetInput().z));
        break;
    case GLFW_KEY_S:
        playerController->SetInput(vec3(playerController->GetInput().x,playerController->GetInput().y, 0));
        break;
    case GLFW_KEY_W:
        playerController->SetInput(vec3(playerController->GetInput().x,playerController->GetInput().y, 0));
        break;
    default: 
        break;
    }
}

void InputManager::HandleMouseButtonPress(int key)
{
    if (inputMode == InputMode::UIOnly)
    {
        double xpos, ypos;
        glfwGetCursorPos(Helper::GetWindow(), &xpos, &ypos);
        OnClickDelegate.Execute(vec2(xpos, ypos));
    }
    else if (inputMode == InputMode::GameOnly && key == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (!playerController) return;
        playerController->Shoot();
    }
}

void InputManager::HandleMouseButtonRelease(int key)
{
}

void InputManager::HandleMouseMove(double x, double y)
{
    if (!playerTransform)   return;
    if (inputMode == InputMode::GameOnly || inputMode == InputMode::GameAndUI)
    {
        vec2 pos = {x, y};
        if (firstMouse)
        {
            rotation = playerTransform->rotation;
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
        rotation.pitch += yoffset;

        if(rotation.pitch > 89.0f)
            rotation.pitch = 89.0f;
        if(rotation.pitch < -89.0f)
            rotation.pitch = -89.0f;

        playerTransform->SetRotation(rotation.pitch, rotation.yaw, rotation.roll);
    }
}

void InputManager::HandleMouseScroll(double x, double y)
{
    
}

void InputManager::SetInputMode(InputMode _mode)
{
    inputMode = _mode;
}

void InputManager::EnableInput(bool value)
{
    if (value == isInputEnabled)    return;
    
    if (!value)
    {
        isInputEnabled = false;
        glfwSetInputMode(Helper::GetWindow(), GLFW_STICKY_KEYS, GLFW_FALSE);
        glfwSetInputMode(Helper::GetWindow(), GLFW_STICKY_MOUSE_BUTTONS, GLFW_FALSE);
        // Logger::Log(LogLevel::Info, "Input disabled");
    }
    else
    {
        isInputEnabled = true;
        glfwSetInputMode(Helper::GetWindow(), GLFW_STICKY_KEYS, GLFW_TRUE); 
        glfwSetInputMode(Helper::GetWindow(), GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
        if (inputMode == InputMode::GameOnly)
        {
            glfwSetInputMode(Helper::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else
        {
            glfwSetInputMode(Helper::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        // Logger::Log(LogLevel::Info, "Input enabled");
    }
}

void InputManager::Clear()
{
    playerController = nullptr;
    playerTransform = nullptr;
}

void InputManager::ScapeInput() const 
{
    World::GetInstance()->EndApplication();
}



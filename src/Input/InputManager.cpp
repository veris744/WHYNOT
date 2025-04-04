#include "InputManager.h"

#include "EventsBuffer.h"
#include "Components/PlayerController.h"
#include "Managers/Helper.h"
#include "Managers/World.h"
#include "Components/Transform.h"
#include "Editor/EditorMode.h"
#include "GameManagers/AliensLogic.h"
#include "Physics/CollisionManager.h"
#include "Physics/Hit.h"
#include "Utils/Debugger.h"
#include "Managers/ConfigurationValues.h"


////////////////////////////////////////////////////////
/// INPUT MANAGER DEFINITIONS //////////////////////////
////////////////////////////////////////////////////////

std::shared_ptr<InputManager> InputManager::instance = nullptr;
unordered_map<unsigned int, KeyStatus> InputManager::keysStatus;
std::unique_ptr<EventsBuffer> InputManager::eventsBuffer = std::make_unique<EventsBuffer>();
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
        keysStatus[actionPair.first] = KeyStatus::RELEASED;
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
        keysStatus[key] = KeyStatus::RELEASED;
    }
    else
    {
        event.type = EventType::KeyPress;
        keysStatus[key] = KeyStatus::PRESSED;
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
        keysStatus[button] = KeyStatus::RELEASED;
    }
    else
    {
        event.type = EventType::MouseButtonPress;
        keysStatus[button] = KeyStatus::PRESSED;
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
    if (ConfigurationValues::IsEditorOpen)
    {
        bool inputOpen = EditorMode::isInputBoxOpen;
        EditorMode::ProcessUserInput(key);
        if (inputOpen) return;
    }
    switch(key)
    {
        case GLFW_KEY_ESCAPE:
            ScapeInput();
            break;
        case GLFW_KEY_UP:
            playerController->SetInput(vec3(0,1,0));
            break;
        case GLFW_KEY_DOWN:
            playerController->SetInput(vec3(0,-1,0));
            break;
        case GLFW_KEY_A:
            playerController->SetInput(vec3(-1,0,0));
            break;
        case GLFW_KEY_D:
            playerController->SetInput(vec3(1,0,0));
            break;
        case GLFW_KEY_S:
            playerController->SetInput(vec3(0,0,-1));
            break;
        case GLFW_KEY_W:
            playerController->SetInput(vec3(0,0,1));
            break;
        case GLFW_KEY_1:
            Debugger::SetCollisionDebug(!Debugger::GetCollisionDebugEnabled());
        break;
        case GLFW_KEY_F1:
            EditorMode::EnterEditorMode();
        break;
        case GLFW_KEY_P:
        {
            if (World::GetInstance()->IsPaused())
                World::GetInstance()->Resume();
            else
                World::GetInstance()->Pause();
            ConfigurationValues::CanPlayerMove = !ConfigurationValues::CanPlayerMove;
            ConfigurationValues::CanPlayerLook = !ConfigurationValues::CanPlayerLook;
        }
        break;
        default: 
            break;
    }
}

void InputManager::HandleKeyRelease(int key, int mods)
{
    if (EditorMode::isInputBoxOpen) return;
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
    if (EditorMode::isInputBoxOpen) return;
    
    if (ConfigurationValues::IsUIActive && key == GLFW_MOUSE_BUTTON_1)
    {
        double xpos, ypos;
        glfwGetCursorPos(Helper::GetWindow(), &xpos, &ypos);

        if (ConfigurationValues::IsEditorOpen)
        {
            Hit hit = CollisionManager::ThrowRayFromScreen(vec2{xpos,ypos}, playerTransform->position, false);
            if (hit.hasHit && hit.type == HitType::World)
            {
                hit.entity->OnClicked();
                EditorMode::SelectEntity(hit.entity);
            }
            else if (!hit.hasHit)
            {
                EditorMode::Unselect();
            }
        }
        else
        {
            CollisionManager::CheckUIClicked(vec2{xpos,ypos});
        }
    }
    else if (ConfigurationValues::ActiveGame == "Aliens" && key == GLFW_MOUSE_BUTTON_1)
    {
        if (!playerController) return;
        playerController->Shoot();
    }
    else if (ConfigurationValues::IsEditorOpen && key == GLFW_MOUSE_BUTTON_2)
    {
        ConfigurationValues::CanPlayerLook = true;
        firstMouse = true;
    }
}

void InputManager::HandleMouseButtonRelease(int key)
{
    if (ConfigurationValues::IsEditorOpen && key == GLFW_MOUSE_BUTTON_2)
    {
        ConfigurationValues::CanPlayerLook = false;
        firstMouse = true;
    }
}

void InputManager::HandleMouseMove(double x, double y)
{
    if (EditorMode::isInputBoxOpen) return;
    if (!playerTransform)   return;
    if (ConfigurationValues::CanPlayerLook)
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

        rotation.yaw += xoffset;
        rotation.pitch -= yoffset;

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

void InputManager::EnableInput(bool value)
{
    if (value == isInputEnabled)    return;
    
    if (!value)
    {
        isInputEnabled = false;
        glfwSetInputMode(Helper::GetWindow(), GLFW_STICKY_KEYS, GLFW_FALSE);
        glfwSetInputMode(Helper::GetWindow(), GLFW_STICKY_MOUSE_BUTTONS, GLFW_FALSE);
    }
    else
    {
        isInputEnabled = true;
        glfwSetInputMode(Helper::GetWindow(), GLFW_STICKY_KEYS, GLFW_TRUE); 
        glfwSetInputMode(Helper::GetWindow(), GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
        if (!ConfigurationValues::IsUIActive)
        {
            glfwSetInputMode(Helper::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else
        {
            glfwSetInputMode(Helper::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}

void InputManager::Clear()
{
    playerController = nullptr;
    playerTransform = nullptr;
}

void InputManager::ScapeInput() const 
{
    if (ConfigurationValues::IsEditorOpen)
    {
        if (EditorMode::GetSelectedEntity())
            EditorMode::Unselect();
        else
            EditorMode::ExitEditorMode();
        return;
    }

    if (World::GetCurrentScene() == "MainMenu")
        World::GetInstance()->EndApplication();
    else
        World::GetInstance()->LoadScene("MainMenu");
}

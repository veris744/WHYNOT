#pragma once
#include <glad/include/glad/glad.h>
#include <glfw/glfw3.h>
#include <unordered_map>
#include <Utils/Rotation.h>

#include "Utils/MultiDelegate.h"
#include "Utils/SingleDelegate.h"

class PlayerController;
class Entity;
class EventsBuffer;
class Transform;

enum class KeyStatus
{
    PRESSED, RELEASED
};

enum class Action
{
    MoveForward, MoveBackwards, MoveRight, MoveLeft, MoveUp, MoveDown, Jump,
    Quit, LeftClick, RightClick,
    OpenEditorMode, DebugCollision, DebugTransforms
};

static std::unordered_map<unsigned int, Action> actions =
{
    {GLFW_KEY_W, Action::MoveForward},
    {GLFW_KEY_S, Action::MoveBackwards},
    {GLFW_KEY_D, Action::MoveRight},
    {GLFW_KEY_A, Action::MoveLeft},
    {GLFW_KEY_UP, Action::MoveUp},
    {GLFW_KEY_SPACE, Action::Jump},
    {GLFW_KEY_DOWN, Action::MoveDown},
    {GLFW_KEY_ESCAPE, Action::Quit},
    {GLFW_MOUSE_BUTTON_1, Action::LeftClick},
    {GLFW_MOUSE_BUTTON_2, Action::RightClick},
    {GLFW_KEY_F1, Action::OpenEditorMode},
    {GLFW_KEY_1, Action::DebugCollision},
    {GLFW_KEY_2, Action::DebugTransforms},
};

class InputManager
{
    static std::shared_ptr<InputManager> instance;
    InputManager() = default;

    static void InitKeys();
    static std::unordered_map<unsigned int, KeyStatus> keysStatus;
    static std::unique_ptr<EventsBuffer> eventsBuffer;
    
    glm::vec3 movementInputPlaneValue = glm::vec3(0, 0, 0);
    glm::vec2 movementInputAxisValue = glm::vec2(0, 0);
    
    void ScapeInput();

    //USEFUL ASSETS
    PlayerController* playerController;
    Transform* playerTransform;

    static bool isInputEnabled;
    
public:
    static std::shared_ptr<InputManager> GetInstance();
    // void Initialize();
    void Update(float _deltaTime);
    void ProcessInput();
    void ProcessPlayerInput() const;

    static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
    bool firstMouse = true;
    glm::vec2 lastMousePos = glm::vec2(0, 0);
    Rotation rotation;

    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    void HandleKeyPress(int key, int mods);
    void HandleKeyRelease(int key, int mods);
    void HandleMouseButtonPress(int key);
    void HandleMouseButtonRelease(int key);
    void HandleMouseMove(double x, double y);
    void HandleMouseScroll(double x, double y);

    static void EnableInput(bool value);

    void Clear();
};

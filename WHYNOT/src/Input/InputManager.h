#pragma once
#include <includes.h>
#include <unordered_map>
#include <Utils/Rotation.h>

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
    MoveForward, MoveBackwards, MoveRight, MoveLeft, MoveUp, MoveDown, Quit, Click
};

enum class InputMode
{
    UIOnly, GameOnly, GameAndUI 
};

static unordered_map<Action, unsigned int> actions =
{
    {Action::MoveForward, GLFW_KEY_W},
    {Action::MoveBackwards, GLFW_KEY_S},
    {Action::MoveRight, GLFW_KEY_D},
    {Action::MoveLeft, GLFW_KEY_A}, 
    {Action::MoveUp, GLFW_KEY_UP},
    {Action::MoveDown, GLFW_KEY_DOWN},
    {Action::Quit, GLFW_KEY_ESCAPE},
    {Action::Click, GLFW_MOUSE_BUTTON_1},
};

class InputManager
{
    static std::shared_ptr<InputManager> instance;
    InputManager() = default;

    static void InitKeys();
    static unordered_map<unsigned int, KeyStatus> keysStatus;
    static std::unique_ptr<EventsBuffer> eventsBuffer;

    static InputMode inputMode;
    
    vec3 movementInputPlaneValue = vec3(0, 0, 0);
    vec2 movementInputAxisValue = vec2(0, 0);
    
    void ScapeInput() const;

    //USEFUL ASSETS
    std::shared_ptr<PlayerController> playerController;
    std::shared_ptr<Transform> playerTransform;

    static bool isInputEnabled;
    
public:
    static std::shared_ptr<InputManager> GetInstance();
    // void Initialize();
    void Update(float _deltaTime);
    void ProcessInput();

    static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
    bool firstMouse = true;
    vec2 lastMousePos = vec2(0, 0);
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

    static void SetInputMode(InputMode _mode);
    static InputMode GetInputMode() { return inputMode; };
    static void EnableInput(bool value);
    
    SingleDelegate<vec2> OnClickDelegate;

    void Clear();
};

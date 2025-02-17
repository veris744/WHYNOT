#include "LoadSceneButton.h"

#include "Managers/World.h"

unsigned int LoadSceneButton::counter = 0;

LoadSceneButton::LoadSceneButton(const vec2& _pos, const vec2& _size, const string& _name)
    : Button(_pos, _size)
{
    name = _name.empty() ? "StartButton" + std::to_string(++counter) : _name;
}

void LoadSceneButton::SetAutoName()
{
    if (name.empty())
        name = "StartButton" + std::to_string(++counter);
}

void LoadSceneButton::OnClick(vec2 _mousePos)
{
    Button::OnClick(_mousePos);
    
    if (sceneName.empty())
    {
        Logger::Log(LogLevel::Warning, "No scene name associated to " + name);
        return;
    }
    World::GetInstance()->LoadScene(sceneName);
}

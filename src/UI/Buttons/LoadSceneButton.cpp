#include "LoadSceneButton.h"

#include "Managers/World.h"


LoadSceneButton::LoadSceneButton(const glm::vec2& _pos, const glm::vec2& _size, const std::string& _name)
    : Button(_pos, _size, _name)
{
}

void LoadSceneButton::OnClick(glm::vec2 _mousePos)
{
    Button::OnClick(_mousePos);
    
    if (sceneName.empty())
    {
        Logger::Log(LogLevel::Warning, "No scene name associated to " + name);
        return;
    }
    World::GetInstance()->LoadScene(sceneName);
}

#include "ShowPropertiesButton.h"

#include <Editor/EditorMode.h>

ShowPropertiesButton::ShowPropertiesButton(const glm::vec2& _pos, const glm::vec2& _size, Entity* _entity,  const std::string& _name)
    : Button(_pos, _size, _name), entity(_entity)
{
}

void ShowPropertiesButton::Initialize()
{
    Button::Initialize();
}

void ShowPropertiesButton::OnClick(glm::vec2 _mousePos)
{
    Button::OnClick(_mousePos);
    EditorMode::SelectEntity(entity);
}

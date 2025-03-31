#include "ShowPropertiesButton.h"

#include <Editor/EditorMode.h>

ShowPropertiesButton::ShowPropertiesButton(const vec2& _pos, const vec2& _size, Entity* _entity,  const string& _name)
    : Button(_pos, _size, _name), entity(_entity)
{
}

void ShowPropertiesButton::Initialize()
{
    Button::Initialize();
}

void ShowPropertiesButton::OnClick(vec2 _mousePos)
{
    Button::OnClick(_mousePos);
    EditorMode::SelectEntity(entity);
}

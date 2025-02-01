#include "Button.h"

#include "Image2D.h"
#include "Graphics/Texture.h"
#include "Input/InputManager.h"

unsigned int Button::counter = 0;

Button::Button(const vec2& _pos, const vec2& _size)
    : Widget(_pos, _size)
{
    name = "Button" + std::to_string(++counter);
    InputManager::GetInstance()->OnClickDelegate.Bind(&Button::OnClickTrigger, this);
}

Button::Button(const string& _name, const vec2& _pos, const vec2& _size)
    : Widget(_name, _pos, _size)
{
    InputManager::GetInstance()->OnClickDelegate.Bind(&Button::OnClickTrigger, this);
}

bool Button::IsClicking(const vec2& _mousePos) const
{
    if (_mousePos.x >= position.x && _mousePos.x <= position.x + size.x &&
        _mousePos.y >= position.y && _mousePos.y <= position.y + size.y)
    {
        return true;
    }
    return false;
}

void Button::OnClickTrigger(vec2 _mousePos)
{
    if (!IsClicking(_mousePos)) return;
    OnClick(_mousePos);
}

void Button::OnClick(vec2 _mousePos)
{
    Logger::Log<Button>(LogLevel::Info, "Click");
}

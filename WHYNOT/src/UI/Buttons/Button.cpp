#include "Button.h"

#include "Input/InputManager.h"

unsigned int Button::counter = 0;

Button::Button(const vec2& _pos, const vec2& _size, const string& _name)
    : Widget(_pos, _size)
{
    name = _name.empty() ? "Button" + std::to_string(++counter) : _name;
    InputManager::GetInstance()->OnClickDelegate.Add(&Button::OnClickTrigger, this);
}

Button::Button()
{
    InputManager::GetInstance()->OnClickDelegate.Add(&Button::OnClickTrigger, this);
}

void Button::SetAutoName()
{
    if (name.empty())
        name = "Button" + std::to_string(++counter);
}

bool Button::IsClicking(const vec2& _mousePos) const
{
    vec2 pixelPos = GetPixelPosition();
    if (_mousePos.x >= pixelPos.x - size.x * 0.5 && _mousePos.x <= pixelPos.x + size.x * 0.5 &&
        _mousePos.y >= pixelPos.y - size.y * 0.5 && _mousePos.y <= pixelPos.y + size.y * 0.5)
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

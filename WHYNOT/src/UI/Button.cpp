#include "Button.h"

#include "Image2D.h"
#include "Graphics/Texture.h"
#include "Input/InputManager.h"

Button::Button(const vec2& _pos, const vec2& _size)
{
    position = _pos;
    size = _size;
    InputManager::GetInstance()->OnClickDelegate.Bind(&Button::OnClick, this);
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

void Button::OnClick(vec2 _mousePos)
{
    if (IsClicking(_mousePos))
    {
        Logger::Log<Button>(LogLevel::Info, "Click");
    }
}

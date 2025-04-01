#include "Button.h"

#include "Input/InputManager.h"

unsigned int Button::counter = 0;

Button::Button(const vec2& _pos, const vec2& _size, const string& _name)
    : Widget(_pos, _size)
{
    name = _name.empty() ? "Button" + std::to_string(++counter) : _name;
}

void Button::Initialize()
{
    Widget::Initialize();
    isBlocking = true;
}

void Button::SetAutoName()
{
    if (name.empty())
        name = "Button" + std::to_string(++counter);
}

void Button::OnClick(vec2 _mousePos)
{
    // Logger::Log<Button>(LogLevel::Info, "Click");
}

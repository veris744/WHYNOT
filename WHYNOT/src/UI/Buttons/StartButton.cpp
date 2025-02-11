#include "StartButton.h"

#include "Managers/World.h"

unsigned int StartButton::counter = 0;

StartButton::StartButton(const vec2& _pos, const vec2& _size, const string& _name)
    : Button(_pos, _size)
{
    name = _name.empty() ? "StartButton" + std::to_string(++counter) : _name;
}

void StartButton::OnClick(vec2 _mousePos)
{
    Button::OnClick(_mousePos);
    World::GetInstance()->LoadScene("Aliens");
}

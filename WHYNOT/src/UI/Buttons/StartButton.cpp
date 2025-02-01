#include "StartButton.h"

#include "Managers/World.h"

unsigned int StartButton::counter = 0;

StartButton::StartButton(const vec2& _pos, const vec2& _size)
    : Button(_pos, _size)
{
    name = "StartButton" + std::to_string(++counter);
}

StartButton::StartButton(const string& _name, const vec2& _pos, const vec2& _size)
    : Button(_name, _pos, _size)
{}

void StartButton::OnClick(vec2 _mousePos)
{
    Button::OnClick(_mousePos);
    World::GetInstance()->StartGame();
}

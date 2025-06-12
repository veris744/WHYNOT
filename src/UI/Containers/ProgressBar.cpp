//
// Created by Isa on 12/06/2025.
//

#include "ProgressBar.h"

#include "Panel.h"

unsigned int ProgressBar::counter = 0;

ProgressBar::ProgressBar(vec2 _pos, vec2 _size, const string& _name)
    : Widget(_pos, _size)
{
    name = _name.empty() ? "ProgressBar" + std::to_string(++counter) : _name;
}

void ProgressBar::Render()
{
    Widget::Render();
}

void ProgressBar::Clear()
{
    Widget::Clear();
}

void ProgressBar::SetAutoName()
{
    if (name.empty())
        name = "ProgressBar" + std::to_string(++counter);
}

void ProgressBar::UpdateValue(float _value)
{
    _value = std::clamp(_value, 0.f, 100.f);

    value = _value * 0.01;

    frontPanel->size = vec2(size.x, size.y * value);
    frontPanel->position = vec2(0, size.y * 0.5f - (frontPanel->size.y * 0.5f));

    frontPanel->SetPixelPosition();
}

void ProgressBar::Initialize()
{
    Widget::Initialize();

    std::shared_ptr<Panel> background = std::make_shared<Panel>();
    background->background = vec4{0, 0, 1, 1};
    background->size = size;
    background->position = vec2{0, 0};
    AddWidget(background);


    std::shared_ptr<Panel> front = std::make_shared<Panel>();
    front->background = vec4{1, 0, 0, 1};
    front->layer = 1;
    AddWidget(front);

    frontPanel = front.get();

    UpdateValue(value);
}


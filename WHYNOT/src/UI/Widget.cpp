#include "Widget.h"

#include "Managers/Helper.h"


void Widget::AddWidget(std::shared_ptr<Widget> _widget)
{
    _widget->finalPos = _widget->position + position;
    _widget->SetInputMode(mode);
    children.push_back(_widget);
}

void Widget::Render()
{
    for (const auto& widget : children)
    {
        widget->Render();
    }
}

void Widget::Clear()
{
    for (const auto& widget : children)
    {
        widget->Clear();
    }
}

vec2 Widget::GetPixelPosition() const
{
    float x = Helper::windowWidth * finalPos.x * 0.01 - size.x * 0.5;
    float y = Helper::windowHeight * finalPos.y * 0.01 - size.y * 0.5;
    return vec2(x, y);
}

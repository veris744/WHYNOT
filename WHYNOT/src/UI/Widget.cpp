#include "Widget.h"


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

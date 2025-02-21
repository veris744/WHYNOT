#include "Widget.h"

#include "Managers/Helper.h"
#include "Managers/World.h"


void Widget::AddWidget(std::shared_ptr<Widget> _widget)
{
    _widget->parent = shared_from_this();
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

void Widget::Destroy()
{
    World::GetInstance()->MarkForDestruction(shared_from_this());
}

vec2 Widget::GetPixelPosition() const
{
    if (!parent)
    {
        float x = Helper::windowWidth * position.x * 0.01f ;
        float y = Helper::windowHeight * position.y * 0.01f;
        return vec2(x, y);
    }

    vec2 parentPos = parent->GetPixelPosition();
    float parentWidth = parent->size.x;
    float parentHeight = parent->size.y;

    float x = parentPos.x + parentWidth * position.x * 0.01f;
    float y = parentPos.y + parentHeight * position.y * 0.01f;

    return vec2(x, y);
}

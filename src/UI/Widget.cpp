#include "Widget.h"

#include "Managers/Helper.h"
#include "Managers/World.h"
#include "Utils/Parser.h"


void Widget::AddWidget(const std::shared_ptr<Widget>& _widget)
{
    _widget->parent = this;
    children.push_back(_widget);
    _widget->Initialize();
}

bool Widget::IsClicking(const vec2& _mousePos) const
{
    if (_mousePos.x >= pixelPosition.x - size.x * 0.5 && _mousePos.x <= pixelPosition.x + size.x * 0.5 &&
        _mousePos.y >= pixelPosition.y - size.y * 0.5 && _mousePos.y <= pixelPosition.y + size.y * 0.5)
    {
        return true;
    }
    return false;
}

void Widget::Render()
{
    size = GetAutoSize();
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

void Widget::SetPixelPosition()
{
    if (!parent)
    {
        float x = Helper::windowWidth * position.x * 0.01f ;
        float y = Helper::windowHeight * position.y * 0.01f;
        
        x += pixelCorrection.x;
        y += pixelCorrection.y;

        pixelPosition = {x, y};
        return;
    }

    vec2 parentPos = parent->GetPixelPosition();
    float parentWidth = parent->size.x;
    float parentHeight = parent->size.y;

    float x = parentPos.x + parentWidth * position.x * 0.01f;
    float y = parentPos.y + parentHeight * position.y * 0.01f;
    
    x += pixelCorrection.x;
    y += pixelCorrection.y;
    
    pixelPosition = {x, y};

    for (const auto& widget : children)
    {
        widget->SetPixelPosition();
    }
}

// vec2 Widget::GetPixelPosition() const
// {
//     if (!parent)
//     {
//         float x = Helper::windowWidth * position.x * 0.01f ;
//         float y = Helper::windowHeight * position.y * 0.01f;
//         
//         x += pixelCorrection.x;
//         y += pixelCorrection.y;
//
//         return {x, y};
//     }
//
//     vec2 parentPos = parent->GetPixelPosition();
//     float parentWidth = parent->size.x;
//     float parentHeight = parent->size.y;
//
//     float x = parentPos.x + parentWidth * position.x * 0.01f;
//     float y = parentPos.y + parentHeight * position.y * 0.01f;
//     
//     x += pixelCorrection.x;
//     y += pixelCorrection.y;
//     
//     return {x, y};
// }

vec2 Widget::GetAutoSize() const
{
    vec2 autoSize = size;
    if (autoSizing == AutoSizing::HORIZONTAL || autoSizing == AutoSizing::ALL)
    {
        if (parent)
            autoSize.x = parent->size.x;
        else
            autoSize.x = Helper::windowWidth;
    }
    if (autoSizing == AutoSizing::VERTICAL || autoSizing == AutoSizing::ALL)
    {
        if (parent)
            autoSize.y = parent->size.y;
        else
            autoSize.y = Helper::windowHeight;
    }
    return autoSize;
}

void Widget::OnWindowResize()
{
    size = GetAutoSize();
    SetPixelPosition();
}

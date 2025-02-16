#include "Text.h"

#include "Managers/TextRenderer.h"

unsigned int Text::counter = 0;

void Text::Render()
{
    Widget::Render();
    vec2 pixelPos = GetPixelPosition();
    TextRenderer::GetInstance()->RenderText(text, pixelPos.x, pixelPos.y, scale, color);
}

void Text::Clear()
{
    Widget::Clear();
    
    TextRenderer::GetInstance()->Clear();
}

void Text::SetAutoName()
{
    if (name.empty())
        name = "Text" + std::to_string(++counter);
}

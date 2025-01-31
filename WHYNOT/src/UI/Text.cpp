#include "Text.h"

#include "Managers/TextRenderer.h"

void Text::Render()
{
    Widget::Render();

    TextRenderer::GetInstance()->RenderText(text, position.x, position.y, scale, color);
}

void Text::Clear()
{
    Widget::Clear();
    
    TextRenderer::GetInstance()->Clear();
}

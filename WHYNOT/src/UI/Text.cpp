#include "Text.h"

#include "Managers/TextRenderer.h"

unsigned int Text::counter = 0;

void Text::Render()
{
    Widget::Render();

    TextRenderer::GetInstance()->RenderText(text, finalPos.x, finalPos.y, scale, color);
}

void Text::Clear()
{
    Widget::Clear();
    
    TextRenderer::GetInstance()->Clear();
}

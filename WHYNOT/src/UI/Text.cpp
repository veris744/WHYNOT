#include "Text.h"

#include "Managers/Helper.h"
#include "Managers/TextRenderer.h"

unsigned int Text::counter = 0;

vec2 Text::CalculateSize()
{
    auto map = TextRenderer::GetInstance()->GetCharacters();

    float width = 0.0f;
    float maxHeight = 0.0f;

    for (char c : text)
    {
        if (map.contains(c))
        {
            Character ch = map[c];
            width += (ch.Advance >> 6);
            if (ch.Size.y > maxHeight)
                maxHeight = ch.Size.y;
        }
    }
    return vec2(width * scale, maxHeight * scale);
}

void Text::Render()
{
    Widget::Render();
    vec2 pixelPos = GetPixelPosition();
    vec2 parentSize = parent ? parent->size : vec2(Helper::windowWidth, Helper::windowHeight);
    switch (align)
    {
        case TextAlign::LEFT:
            pixelPos.x = pixelPos.x - parentSize.x * 0.5f + padding.x;
        break;
        case TextAlign::CENTER:
            pixelPos.x = pixelPos.x - size.x * 0.5f;
        break;
        case TextAlign::RIGHT:
            pixelPos.x = pixelPos.x + parentSize.x * 0.5f - size.x - padding.x;
    }
    switch (alignVertical)
    {
    case TextAlignVertical::TOP:
        pixelPos.y = pixelPos.y - parentSize.y * 0.5f + padding.y;
        break;
    case TextAlignVertical::CENTER:
        pixelPos.y = pixelPos.y - size.y * 0.5f;
        break;
    case TextAlignVertical::BOTTOM:
        pixelPos.y = pixelPos.y + parentSize.y * 0.5f - size.y - padding.y;
    }
    
    TextRenderer::GetInstance()->RenderText(text, pixelPos.x, pixelPos.y, scale, color, layer);
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

void Text::SetText(const string& _text)
{
    text = _text;
    size = CalculateSize();
}

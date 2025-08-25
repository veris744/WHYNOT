#pragma once

#include "UI/Widget.h"

enum class TextAlign
{
    LEFT, CENTER, RIGHT
};
REGISTER_ENUM(TextAlign,
    {"LEFT", TextAlign::LEFT},
    {"CENTER", TextAlign::CENTER},
    {"RIGHT", TextAlign::RIGHT},
);
enum class TextAlignVertical
{
    TOP, CENTER, BOTTOM
};
REGISTER_ENUM(TextAlignVertical,
    {"TOP", TextAlignVertical::TOP},
    {"CENTER", TextAlignVertical::CENTER},
    {"BOTTOM", TextAlignVertical::BOTTOM},
);


class Text : public Widget
{
    static unsigned int counter;
    
public:
    Text(): color(glm::vec3(0, 0, 0)), scale(0.5f), align(TextAlign::LEFT), alignVertical(TextAlignVertical::CENTER), padding()
    {
        hasTransparency = true;
    }

    Text(const std::string& _text, glm::vec3 _color, float _scale, glm::vec2 _position, const std::string& _name = "")
    {
        name = _name.empty() ? "Text" + std::to_string(++counter) : _name;
        text = _text;
        color = _color;
        scale = _scale;
        position = _position;
        align = TextAlign::LEFT;
        alignVertical = TextAlignVertical::CENTER;
        size = {0,0};
        padding = glm::vec2(0);
        hasTransparency = true;
    }

    std::string text;
    glm::vec3 color;
    float scale;
    TextAlign align;
    TextAlignVertical alignVertical;
    glm::vec2 padding;

    void Initialize() override;
    glm::vec2 CalculateSize();
    void Render() override;
    void Clear() override;
    void SetAutoName() override;

    void SetText(const std::string& _text);
};
REGISTER_CLASS(Text, {
    REGISTER_MEMBER(Text, name, MemberProperty::Viewable),
    REGISTER_MEMBER(Text, text, MemberProperty::Viewable),
    REGISTER_MEMBER(Text, scale, MemberProperty::Viewable),
    REGISTER_MEMBER(Text, color, MemberProperty::Viewable),
    REGISTER_MEMBER(Text, position, MemberProperty::Viewable),
    REGISTER_MEMBER(Text, align, MemberProperty::Viewable),
    REGISTER_MEMBER(Text, alignVertical, MemberProperty::Viewable),
    REGISTER_MEMBER(Text, pixelCorrection, MemberProperty::Viewable),
    REGISTER_MEMBER(Text, padding, MemberProperty::Viewable)
});
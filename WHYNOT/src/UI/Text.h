#pragma once
#include <includes.h>

#include "Widget.h"

class Text : public Widget
{
    static unsigned int counter;
public:
    Text(): color(vec3(0,0,0)), scale(0){}
    Text(const string& _text, vec3 _color, float _scale, vec2 _position, const string& _name = "")
    {
        name = _name.empty() ? "Text" + std::to_string(++counter) : _name;
        text = _text;
        color = _color;
        scale = _scale;
        position = _position;
        size = vec2(scale);
    }

    string text;
    vec3 color;
    float scale;

    void Render() override;
    void Clear() override;
    void SetAutoName() override;
};
REGISTER_CLASS(Text, {
    REGISTER_MEMBER(Text, name),
    REGISTER_MEMBER(Text, text),
    REGISTER_MEMBER(Text, scale),
    REGISTER_MEMBER(Text, color),
    REGISTER_MEMBER(Text, position),
});
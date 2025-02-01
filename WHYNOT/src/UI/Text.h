#pragma once
#include <includes.h>

#include "Widget.h"

class Text : public Widget
{
    static unsigned int counter;
public:
    Text(const string& _text, vec3 _color, float _scale, vec2 _position)
    {
        name = "Text" + std::to_string(++counter);
        text = _text;
        color = _color;
        scale = _scale;
        position = _position;
    }
    Text(const string& _name, const string& _text, vec3 _color, float _scale, vec2 _position)
    {
        name = _name;
        text = _text;
        color = _color;
        scale = _scale;
        position = _position;
    }
    
    string text;
    vec3 color;
    float scale;

    void Render() override;
    void Clear() override;
};

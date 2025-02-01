#pragma once
#include "Widget.h"

class Image2D;

class Button : public Widget
{
    static unsigned int counter;
public:
    Button(const vec2& _pos, const vec2& _size);
    Button(const string& _name, const vec2& _pos, const vec2& _size);

    bool IsClicking(const vec2& _mousePos) const;

    void OnClickTrigger(vec2 _mousePos);
    virtual void OnClick(vec2 _mousePos);
};

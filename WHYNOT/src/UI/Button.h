#pragma once
#include "Widget.h"

class Image2D;

class Button : public Widget
{
public:
    Button(const vec2& _pos, const vec2& _size);

    bool IsClicking(const vec2& _mousePos) const;

    void OnClick(vec2 _mousePos);
};

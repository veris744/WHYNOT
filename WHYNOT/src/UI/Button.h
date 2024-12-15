#pragma once
#include "Widget.h"

class Image2D;

class Button : public Widget
{
    std::shared_ptr<Image2D> image;

    
public:
    Button(const vec2& _pos, const vec2& _size);
    Button(const string& _imagePath, const vec2& _pos, const vec2& _size);

    bool IsClicking(const vec2& _mousePos) const;

    void OnClick(vec2 _mousePos);
};

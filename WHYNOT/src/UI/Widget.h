#pragma once
#include <includes.h>

class Widget
{
protected:
    vec2 position;
    vec2 size;

    vec2 finalPos;

    vector<std::shared_ptr<Widget>> children;
    
public:
    Widget() = default;
    Widget(vec2 _pos, vec2 _size) : position(_pos), size(_size), finalPos(_pos)
    { }

    virtual ~Widget() = default;

    void AddWidget(std::shared_ptr<Widget> _widget);
    virtual void Render();
    virtual void Clear();
    
};

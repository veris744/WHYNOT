#pragma once
#include <includes.h>

class Widget
{
protected:
    vec2 position;
    vec2 size;

    vector<std::shared_ptr<Widget>> children;
    
public:
    virtual ~Widget() = default;
    Widget() = default;

    void AddWidget(std::shared_ptr<Widget> _widget);
    virtual void Render();
    virtual void Clear();
    
};

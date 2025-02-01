#pragma once
#include "Widget.h"

class Panel : public Widget
{
    static unsigned int counter;
public:
    Panel(vec2 _pos)
        : Widget(_pos, vec2(0))
    {
        name = "Panel" + std::to_string(counter++);
    }
    
    Panel(const string& _name, vec2 _pos)
        : Widget(_name, _pos, vec2(0))
    {}
};

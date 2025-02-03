#pragma once
#include "Widget.h"

class Panel : public Widget
{
    static unsigned int counter;
public:
    Panel(vec2 _pos, const string& _name = "")
        : Widget(_pos, vec2(0))
    {
        name = _name.empty() ? "Panel" + std::to_string(++counter) : _name;
    }
};

#pragma once
#include "Button.h"

class StartButton : public Button
{
    static unsigned int counter;
public:
    StartButton(const vec2& _pos, const vec2& _size, const string& _name = "");
    void OnClick(vec2 _mousePos) override;
};

#pragma once
#include "UI/Button.h"

class StartButton : public Button
{
    static unsigned int counter;
public:
    StartButton(const vec2& _pos, const vec2& _size);
    StartButton(const string& _name, const vec2& _pos, const vec2& _size);
    void OnClick(vec2 _mousePos) override;
};

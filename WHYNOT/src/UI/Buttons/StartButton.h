#pragma once
#include "Button.h"

class StartButton : public Button
{
    static unsigned int counter;
public:
    StartButton(const vec2& _pos, const vec2& _size, const string& _name = "");
    StartButton(){};
    void SetAutoName() override;
    void OnClick(vec2 _mousePos) override;
};
REGISTER_CLASS(StartButton, {
    REGISTER_MEMBER(StartButton, name),
    REGISTER_MEMBER(StartButton, position),
    REGISTER_MEMBER(StartButton, size),
});

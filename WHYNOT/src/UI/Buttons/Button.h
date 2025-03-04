#pragma once
#include "UI/Widget.h"

class Image2D;

class Button : public Widget
{
    static unsigned int counter;
public:
    Button(const vec2& _pos, const vec2& _size, const string& _name = "");
    Button();
    void SetAutoName() override;

    bool IsClicking(const vec2& _mousePos) const;

    void OnClickTrigger(vec2 _mousePos);
    virtual void OnClick(vec2 _mousePos);
};
REGISTER_CLASS(Button, {
    REGISTER_MEMBER(Button, name, MemberProperty::Viewable),
    REGISTER_MEMBER(Button, position, MemberProperty::Viewable),
    REGISTER_MEMBER(Button, size, MemberProperty::Viewable),
});
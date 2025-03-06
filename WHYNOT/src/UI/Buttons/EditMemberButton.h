#pragma once
#include "Button.h"

class EditMemberButton : public Button
{
public:
    EditMemberButton(const vec2& _pos, const vec2& _size, const string& _name = "");
    EditMemberButton() = default;
    void OnClick(vec2 _mousePos) override;
    void UpdateMember(const string& _value);
};

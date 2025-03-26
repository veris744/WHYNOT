#pragma once
#include "Button.h"

class MemberView;

class EditMemberButton : public Button
{
    std::shared_ptr<MemberView> memberView;

public:
    EditMemberButton(const vec2& _pos, const vec2& _size, const string& _name = "");
    EditMemberButton() = default;
    void OnClick(vec2 _mousePos) override;
};

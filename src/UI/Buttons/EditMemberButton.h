#pragma once
#include "Button.h"

class MemberView;

class EditMemberButton : public Button
{
    std::shared_ptr<MemberView> memberView;

public:
    EditMemberButton(const glm::vec2& _pos, const glm::vec2& _size, const std::string& _name = "");
    EditMemberButton() = default;
    void OnClick(glm::vec2 _mousePos) override;
};

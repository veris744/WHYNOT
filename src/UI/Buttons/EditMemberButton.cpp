#include "EditMemberButton.h"

#include "Editor/EditorMode.h"
#include "UI/Containers/MemberView.h"

class MemberView;

EditMemberButton::EditMemberButton(const glm::vec2& _pos, const glm::vec2& _size, const std::string& _name)
    : Button(_pos, _size, _name)
{
}

void EditMemberButton::OnClick(glm::vec2 _mousePos)
{
    Button::OnClick(_mousePos);
    memberView = std::make_shared<MemberView>(*static_cast<MemberView*>(parent));
    EditorMode::OnEnterInput.Bind(&MemberView::UpdateMember, memberView.get());
    EditorMode::OpenInputBox();
}


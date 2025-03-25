#include "EditMemberButton.h"

#include "Editor/EditorMode.h"
#include "UI/Containers/MemberView.h"

class MemberView;

EditMemberButton::EditMemberButton(const vec2& _pos, const vec2& _size, const string& _name)
    : Button(_pos, _size, _name)
{
}

void EditMemberButton::OnClick(vec2 _mousePos)
{
    Button::OnClick(_mousePos);
    MemberView* memberView = static_cast<MemberView*>(parent);
    EditorMode::OnEnterInput.Bind(&MemberView::UpdateMember, memberView);
    EditorMode::OpenInputBox();
}


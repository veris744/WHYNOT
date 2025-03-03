#include "MemberView.h"

#include "UI/Text.h"

unsigned int MemberView::counter = 0;

void MemberView::Initialize()
{
    Widget::Initialize();

    std::shared_ptr<Text> memberInfo = std::make_shared<Text>("", color, scale, vec2(0));
    text = memberInfo.get();
    AddWidget(memberInfo);
}

MemberView::MemberView(vec2 _pos, vec2 _size, const string& _name)
    : Widget(_pos, _size)
{
    name = _name.empty() ? "MemberView" + std::to_string(++counter) : _name;
}

void MemberView::SetAutoName()
{
    if (name.empty())
        name = "MemberView" + std::to_string(++counter);
}

void MemberView::SetMemberInfo(const string& info)
{
    text->SetText(info);
}

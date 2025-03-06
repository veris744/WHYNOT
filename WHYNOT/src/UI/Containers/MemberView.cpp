#include "MemberView.h"

#include "UI/Image2D.h"
#include "UI/Buttons/EditMemberButton.h"
#include "UI/Text/Text.h"
#include "Utils/Parser.h"

unsigned int MemberView::counter = 0;

void MemberView::Initialize()
{
    Widget::Initialize();

    std::shared_ptr<Text> memberInfoText = std::make_shared<Text>("", color, scale, vec2(0));
    textWidget = memberInfoText.get();
    AddWidget(memberInfoText);
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

void MemberView::SetMemberInfo(const MemberInfo& info, Component* _component)
{

    std::shared_ptr<EditMemberButton> buttonTemp = std::make_shared<EditMemberButton>(vec2(100, 50), vec2(30,30));
    std::shared_ptr<Image2D> image2D = std::make_shared<Image2D>("assets/bulb.jpg", vec2(0,0), vec2(0,0));
    image2D->autoSizing = AutoSizing::ALL;
    image2D->color = {1, 0.2, 0.8};
    buttonTemp->AddWidget(image2D);
    button = buttonTemp.get();
    AddWidget(buttonTemp);
    
    string res;
    memberInfo = &info;
    component =  _component;
    
    if (!HasProperty(memberInfo->properties, MemberProperty::Hidden))
    {
        res += memberInfo->name;
    }
    if (HasProperty(memberInfo->properties, MemberProperty::Viewable))
    {
        if (info.getter)
        {
            res += ": " + memberInfo->getter(component);
        }
        else
        {
            res += ": Unknown";
        }
    }
    else
    {
        res.append(": Non Viewable");
    }
    textWidget->SetText(res);
}

void MemberView::SetMemberInfo(const string& value)
{
    textWidget->SetText(value);
}

void MemberView::UpdateMember(const string& value)
{
    Logger::Log(LogLevel::Info, Parser::Parse(value));
}

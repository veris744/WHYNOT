#include "MemberView.h"

#include "Components/CircleCollider.h"
#include "UI/Image2D.h"
#include "UI/Buttons/EditMemberButton.h"
#include "UI/Text/Text.h"
#include "Components/Component.h"
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

void MemberView::SetMemberInfo(const MemberInfo& _info, ReflectedObject* _object)
{
    textWidget->padding = vec2(20, 0);
    string res;
    memberInfo = &_info;
    object =  _object;

    if (!HasProperty(memberInfo->properties, MemberProperty::Hidden))
    {
        res += memberInfo->name;
    }
    if (HasProperty(memberInfo->properties, MemberProperty::Viewable))
    {
        if (_info.getter)
        {
            res += ": " + Parser::ParseValue(memberInfo->getter(_object), memberInfo->type_name);
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
    if (HasProperty(memberInfo->properties, MemberProperty::Editable))
    {
        SetUpdateButton();
    }
    textWidget->SetText(res);
}

void MemberView::SetMemberInfo(const string& value) const
{
    textWidget->SetText(value);
}

void MemberView::UpdateMember(const string& value)
{
    YAML::Node node = YAML::Node(Reader::ConvertMemberToYaml(memberInfo->name, value));
    memberInfo->setter(object, node);
    SetMemberInfo(*memberInfo, object);
}

void MemberView::SetUpdateButton()
{
    std::shared_ptr<EditMemberButton> buttonTemp = std::make_shared<EditMemberButton>(vec2(-50, 0), vec2(12,12));
    std::shared_ptr<Image2D> image2D = std::make_shared<Image2D>("assets/2dImages/edit.png", vec2(0,0), vec2(0,0));
    image2D->autoSizing = AutoSizing::ALL;
    buttonTemp->AddWidget(image2D);
    button = buttonTemp.get();
    AddWidget(buttonTemp);
}

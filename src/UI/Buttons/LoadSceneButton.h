#pragma once
#include "Button.h"

class LoadSceneButton : public Button
{
public:
    LoadSceneButton(const vec2& _pos, const vec2& _size, const string& _name = "");
    LoadSceneButton() = default;
    void OnClick(vec2 _mousePos) override;

    string sceneName = "";
};
REGISTER_CLASS(LoadSceneButton, {
    REGISTER_MEMBER(LoadSceneButton, name, MemberProperty::Viewable),
    REGISTER_MEMBER(LoadSceneButton, position, MemberProperty::Viewable),
    REGISTER_MEMBER(LoadSceneButton, size, MemberProperty::Viewable),
    REGISTER_MEMBER(LoadSceneButton, sceneName, MemberProperty::Viewable),
});

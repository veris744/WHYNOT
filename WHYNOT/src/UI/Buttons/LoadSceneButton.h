#pragma once
#include "Button.h"

class LoadSceneButton : public Button
{
    static unsigned int counter;
public:
    LoadSceneButton(const vec2& _pos, const vec2& _size, const string& _name = "");
    LoadSceneButton(){};
    void SetAutoName() override;
    void OnClick(vec2 _mousePos) override;

    string sceneName = "";
};
REGISTER_CLASS(LoadSceneButton, {
    REGISTER_MEMBER(LoadSceneButton, name),
    REGISTER_MEMBER(LoadSceneButton, position),
    REGISTER_MEMBER(LoadSceneButton, size),
    REGISTER_MEMBER(LoadSceneButton, sceneName),
});

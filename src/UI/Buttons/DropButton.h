#pragma once
#include "Button.h"
#include "Reader/Reader.h"

class Dropbox;

class DropButton : public Button
{
    Dropbox* dropbox = nullptr;
    Image2D* image = nullptr;

public:
    DropButton(const vec2& _pos, const vec2& _size, const string& _name = "");
    DropButton() = default;
    void Initialize() override;
    void OnClick(vec2 _mousePos) override;
};



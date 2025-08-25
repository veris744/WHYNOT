#pragma once
#include "Button.h"
#include "Reader/Reader.h"

class Dropbox;

class DropButton : public Button
{
    Dropbox* dropbox = nullptr;
    Image2D* image = nullptr;

public:
    DropButton(const glm::vec2& _pos, const glm::vec2& _size, const std::string& _name = "");
    DropButton() = default;
    void Initialize() override;
    void OnClick(glm::vec2 _mousePos) override;
};



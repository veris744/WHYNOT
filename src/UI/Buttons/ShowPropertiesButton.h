#pragma once
#include "Button.h"

class ShowPropertiesButton  : public Button
{
    Entity * entity = nullptr;

public:
    ShowPropertiesButton(const glm::vec2& _pos, const glm::vec2& _size, Entity* _entity = nullptr, const std::string& _name = "");
    ShowPropertiesButton() = default;
    void Initialize() override;
    void OnClick(glm::vec2 _mousePos) override;
    void SetEntity(Entity* _entity) { entity = _entity; };
};

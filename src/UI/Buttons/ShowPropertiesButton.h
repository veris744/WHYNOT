#pragma once
#include "Button.h"

class ShowPropertiesButton  : public Button
{
    Entity * entity = nullptr;

public:
    ShowPropertiesButton(const vec2& _pos, const vec2& _size, Entity* _entity = nullptr, const string& _name = "");
    ShowPropertiesButton() = default;
    void Initialize() override;
    void OnClick(vec2 _mousePos) override;
    void SetEntity(Entity* _entity) { entity = _entity; };
};

#pragma once
#include "Button.h"
#include "Reader/Reader.h"

class SerializeButton : public Button
{
    YAML::Node GenerateYamlContent();
    YAML::Node GenerateObjectYAML(ReflectedObject* object);
    string GenerateFile(const YAML::Node& node);

public:
    SerializeButton(const vec2& _pos, const vec2& _size, const string& _name = "");
    SerializeButton() = default;
    void Initialize() override;
    void OnClick(vec2 _mousePos) override;
};



#pragma once
#include "Button.h"
#include "Reader/Reader.h"

class SerializeButton : public Button
{
    YAML::Node GenerateYamlContent();
    YAML::Node GenerateObjectYAML(ReflectedObject* object);
    std::string GenerateFile(const YAML::Node& node);

public:
    SerializeButton(const glm::vec2& _pos, const glm::vec2& _size, const std::string& _name = "");
    SerializeButton() = default;
    void Initialize() override;
    void OnClick(glm::vec2 _mousePos) override;
};



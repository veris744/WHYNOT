#pragma once
#include <memory>
#include <yaml-cpp/yaml.h>

#include "UI/Widget.h"


class Button;
class Image2D;

class WidgetFactory
{
public:
    using WidgetCreator = std::function<const std::shared_ptr<Widget>&
        (const YAML::Node&, const std::shared_ptr<Widget>& parent)>;

private:
    static WidgetFactory* instance;
    std::unordered_map<std::string, WidgetCreator> creators;

    static std::shared_ptr<Image2D> ReadImage(const YAML::Node& node);
    static std::shared_ptr<Button> ReadButton(const YAML::Node& node);
    
public:
    WidgetFactory() = default;
    ~WidgetFactory() = default;

    static WidgetFactory* GetInstance()
    {
        if (instance == nullptr)
        {
            instance = new WidgetFactory();
            instance->WidgetFactorySetup();
        }
        return instance;
    }
    
    void WidgetFactorySetup();
    void RegisterWidget(const std::string& type, WidgetCreator creator);
    std::shared_ptr<Widget> CreateWidget(const std::string& type, const YAML::Node& data, const std::shared_ptr<Widget>& parent = nullptr) const;
};

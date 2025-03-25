#pragma once
#include <functional>

#ifndef YAML_CPP_STATIC_DEFINE
#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>
#endif

class Panel;
class Widget;
class Text;
class Button;
class Image2D;

class WidgetFactory
{
    std::shared_ptr<Widget> temp;
public:
    using WidgetCreator = std::function<const std::shared_ptr<Widget>&
        (const YAML::Node&, Widget* parent)>;

private:
    static std::shared_ptr<WidgetFactory> instance;
    std::unordered_map<std::string, WidgetCreator> creators;

    static std::shared_ptr<Button> ReadButton(const YAML::Node& asset);

    static void SaveWidget(const std::shared_ptr<Widget>& widget, Widget* parent);
    
    
public:
    WidgetFactory() = default;
    ~WidgetFactory() = default;

    static std::shared_ptr<WidgetFactory> GetInstance()
    {
        if (instance == nullptr)
        {
            instance = std::make_shared<WidgetFactory>(WidgetFactory());
            instance->WidgetFactorySetup();
        }
        return instance;
    }
    
    void WidgetFactorySetup();
    void RegisterWidget(const std::string& type, WidgetCreator creator);
    std::shared_ptr<Widget> CreateWidget(const std::string& type, const YAML::Node& data, Widget* parent = nullptr) const;
};

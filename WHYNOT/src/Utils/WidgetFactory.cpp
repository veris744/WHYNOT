#include "WidgetFactory.h"

#include "Managers/World.h"
#include "UI/Button.h"
#include "UI/Image2D.h"
#include "UI/Text.h"


using WidgetCreator = std::function<std::shared_ptr<Widget>
    (const YAML::Node&, const std::shared_ptr<Widget>& parent)>;

WidgetFactory* WidgetFactory::instance = nullptr;


void WidgetFactory::WidgetFactorySetup()
{
    RegisterWidget("BUTTON", [](const YAML::Node& asset, const std::shared_ptr<Widget>& parent) ->
        std::shared_ptr<Widget>  {
            std::shared_ptr<Button> widget = ReadButton(asset);
            if (parent)
            {
                parent->AddWidget(widget);
            }
            else
            {
                World::GetInstance()->AddWidget(widget);
            }
            return widget;
        });
    
    RegisterWidget("IMAGE2D", [](const YAML::Node& asset, const std::shared_ptr<Widget>& parent) ->
        std::shared_ptr<Widget> {
            std::shared_ptr<Image2D> widget = ReadImage(asset);
            if (parent)
            {
                parent->AddWidget(widget);
            }
            else
            {
                World::GetInstance()->AddWidget(widget);
            }
            return widget;
    });
    
    RegisterWidget("TEXT", [](const YAML::Node& asset, const std::shared_ptr<Widget>& parent) ->
        std::shared_ptr<Widget> {
            std::shared_ptr<Text> widget = ReadText(asset);
            if (parent)
            {
                parent->AddWidget(widget);
            }
            else
            {
                World::GetInstance()->AddWidget(widget);
            }
            return widget;
        });
    
}

void WidgetFactory::RegisterWidget(const std::string& type, WidgetCreator creator)
{
    creators[type] = std::move(creator);
}



std::shared_ptr<Widget> WidgetFactory::CreateWidget(const std::string& type, const YAML::Node& data, const std::shared_ptr<Widget>& parent) const
{
    auto it = creators.find(type);
    if (it != creators.end()) {
        return it->second(data, parent);
    }
    throw std::runtime_error("Unknown entity type: " + type);
}


std::shared_ptr<Image2D> WidgetFactory::ReadImage(const YAML::Node& asset)
{
    const string& path = asset["path"].as<std::string>();
    vec2 size = vec2(asset["size"][0].as<float>(), asset["size"][1].as<float>());
    vec2 pos = vec2(asset["position"][0].as<float>(), asset["position"][1].as<float>());
    
    std::shared_ptr<Image2D> image = std::make_shared<Image2D>(path, pos, size);
    return image;
}

std::shared_ptr<Button> WidgetFactory::ReadButton(const YAML::Node& asset)
{
    vec2 size = vec2(asset["size"][0].as<float>(), asset["size"][1].as<float>());
    vec2 pos = vec2(asset["position"][0].as<float>(), asset["position"][1].as<float>());
    
    std::shared_ptr<Button> button = std::make_shared<Button>(pos, size);
    return button;
}

std::shared_ptr<Text> WidgetFactory::ReadText(const YAML::Node& asset)
{
    vec2 pos = vec2(asset["position"][0].as<float>(), asset["position"][1].as<float>());
    vec3 color = vec3(asset["color"][0].as<float>(), asset["color"][1].as<float>(), asset["color"][2].as<float>());
    float scale = asset["scale"] ? asset["scale"].as<float>() : 1.0f;
    string text = asset["text"].as<std::string>();

    std::shared_ptr<Text> textWidget = std::make_shared<Text>(text, color, scale, pos);
    return textWidget;
}

#include "WidgetFactory.h"

#include "Managers/World.h"
#include "UI/Image2D.h"
#include "UI/Text.h"
#include "UI/Panel.h"
#include "UI/Buttons/StartButton.h"


using WidgetCreator = std::function<std::shared_ptr<Widget>
    (const YAML::Node&, const std::shared_ptr<Widget>& parent)>;

std::shared_ptr<WidgetFactory> WidgetFactory::instance = nullptr;


void WidgetFactory::WidgetFactorySetup()
{
    RegisterWidget("BUTTON", [](const YAML::Node& asset, const std::shared_ptr<Widget>& parent) ->
        std::shared_ptr<Widget>  {
            std::shared_ptr<Button> widget = ReadButton(asset);
            SaveWidget(widget, parent);
            return widget;
        });
    
    RegisterWidget("IMAGE2D", [](const YAML::Node& asset, const std::shared_ptr<Widget>& parent) ->
        std::shared_ptr<Widget> {
            std::shared_ptr<Image2D> widget = ReadImage(asset);
            SaveWidget(widget, parent);
            return widget;
    });
    
    RegisterWidget("TEXT", [](const YAML::Node& asset, const std::shared_ptr<Widget>& parent) ->
        std::shared_ptr<Widget> {
            std::shared_ptr<Text> widget = ReadText(asset);
            SaveWidget(widget, parent);
            return widget;
        });
    
    RegisterWidget("PANEL", [](const YAML::Node& asset, const std::shared_ptr<Widget>& parent) ->
        std::shared_ptr<Widget> {
            std::shared_ptr<Panel> widget = ReadPanel(asset);
            SaveWidget(widget, parent);
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

void WidgetFactory::SaveWidget(const std::shared_ptr<Widget>& widget, const std::shared_ptr<Widget>& parent)
{
    if (parent)
    {
        for (const auto& sibling : parent->GetChildren())
        {
            if (sibling->GetName() == widget->GetName())
            {
                Logger::Log(LogLevel::Error, "Widget already exists as child: " + widget->GetName());
                return;
            }
        }
        parent->AddWidget(widget);
    }
    else
    {
        for (const auto& sibling : World::GetInstance()->GetWidgets())
        {
            if (sibling->GetName() == widget->GetName())
            {
                Logger::Log(LogLevel::Error, "Widget already exists in world: " + widget->GetName());
                return;
            }
        }
        World::GetInstance()->AddWidget(widget);
    }
}

void WidgetFactory::SetInputMode(const std::shared_ptr<Widget>& widget, const YAML::Node& asset)
{
    // THIS WILL BE OVERWRITTEN BY THE PARENT'S INPUT MODE
    if (asset["inputMode"])
    {
        string mode = asset["inputMode"].as<std::string>();
        if (mode == "UI")
        {
            widget->SetInputMode(InputMode::UIOnly);
        }
        else if (mode == "Game")
        {
            widget->SetInputMode(InputMode::GameOnly);
        }
    }
}


std::shared_ptr<Image2D> WidgetFactory::ReadImage(const YAML::Node& asset)
{
    const string& path = asset["path"].as<std::string>();
    vec2 size = vec2(asset["size"][0].as<float>(), asset["size"][1].as<float>());
    vec2 pos = vec2(0);
    if (asset["position"])
    {
        pos = vec2(asset["position"][0].as<float>(), asset["position"][1].as<float>());
    }
    
    std::shared_ptr<Image2D> image = std::make_shared<Image2D>(path, pos, size);
    if (asset["name"])
    {
        image = std::make_shared<Image2D>(asset["name"].as<string>(), path, pos, size);
    }
    else
    {
        image = std::make_shared<Image2D>(path, pos, size);
    }
    SetInputMode(image, asset);
    return image;
}

std::shared_ptr<Button> WidgetFactory::ReadButton(const YAML::Node& asset)
{
    vec2 size = vec2(asset["size"][0].as<float>(), asset["size"][1].as<float>());
    vec2 pos = vec2(asset["position"][0].as<float>(), asset["position"][1].as<float>());
    
    string subtype = asset["subtype"] ? asset["subtype"].as<string>() : "";
    if (subtype == "Start")
    {
        std::shared_ptr<StartButton> button;
        if (asset["name"])
            button = std::make_shared<StartButton>(asset["name"].as<string>(), pos, size);
        else
            button = std::make_shared<StartButton>(pos, size);
        SetInputMode(button, asset);
        return button;
    }
    else
    {
        std::shared_ptr<Button> button;
        if (asset["name"])
            button = std::make_shared<Button>(asset["name"].as<string>(), pos, size);
        else
            button = std::make_shared<Button>(pos, size);
        SetInputMode(button, asset);
        return button;
    }
    
}

std::shared_ptr<Text> WidgetFactory::ReadText(const YAML::Node& asset)
{
    std::shared_ptr<Text> textWidget;
    
    vec3 color = vec3(asset["color"][0].as<float>(), asset["color"][1].as<float>(), asset["color"][2].as<float>());
    float scale = asset["scale"] ? asset["scale"].as<float>() : 1.0f;
    string text = asset["text"].as<std::string>();
    vec2 pos = vec2(0);
    if (asset["position"])
    {
        pos = vec2(asset["position"][0].as<float>(), asset["position"][1].as<float>());
    }
    if (asset["name"])
    {
        textWidget = std::make_shared<Text>(asset["name"].as<string>(), text, color, scale, pos);
    }
    else
    {
        textWidget = std::make_shared<Text>(text, color, scale, pos);
    }
    SetInputMode(textWidget, asset);
    
    return textWidget;
}

std::shared_ptr<Panel> WidgetFactory::ReadPanel(const YAML::Node& asset)
{
    vec2 pos = vec2(0);
    if (asset["position"])
    {
        pos = vec2(asset["position"][0].as<float>(), asset["position"][1].as<float>());
    }
    std::shared_ptr<Panel> widget;
    if (asset["name"])
    {
        widget = std::make_shared<Panel>(asset["name"].as<string>(), pos);
    }
    else
    {
        widget = std::make_shared<Panel>(pos);
    }
    SetInputMode(widget, asset);
    return widget;
}



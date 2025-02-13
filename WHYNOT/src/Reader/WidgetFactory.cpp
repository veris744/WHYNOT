#include "WidgetFactory.h"

#include "Reader.h"
#include "Managers/World.h"
#include "UI/Image2D.h"
#include "UI/Text.h"
#include "UI/Panel.h"
#include "UI/Buttons/StartButton.h"

using namespace Reader;

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


std::shared_ptr<Image2D> WidgetFactory::ReadImage(const YAML::Node& asset)
{
    const string& path = asset["path"].as<std::string>();
    vec2 size = ReadVec2(asset, "size", vec2(50.f,50.f));
    vec2 pos = ReadVec2(asset, "position");
    vec3 color = ReadVec3(asset, "color");
    
    std::shared_ptr<Image2D> image = std::make_shared<Image2D>(path, pos, size);
    if (asset["name"])
    {
        image = std::make_shared<Image2D>(path, pos, size, ReadString(asset, "name"));
    }
    else
    {
        image = std::make_shared<Image2D>(path, pos, size);
    }
    image->color = color;
    return image;
}

std::shared_ptr<Button> WidgetFactory::ReadButton(const YAML::Node& asset)
{
    vec2 size = ReadVec2(asset, "size", vec2(50.f,50.f));
    vec2 pos = ReadVec2(asset, "position");
    
    string subtype = ReadString(asset, "subtype");
    if (subtype == "Start")
    {
        std::shared_ptr<StartButton> button;
        if (asset["name"])
            button = std::make_shared<StartButton>(pos, size, ReadString(asset, "name"));
        else
            button = std::make_shared<StartButton>(pos, size);
        return button;
    }
    else
    {
        std::shared_ptr<Button> button;
        if (asset["name"])
            button = std::make_shared<Button>(pos, size, ReadString(asset, "name"));
        else
            button = std::make_shared<Button>(pos, size);
        return button;
    }
    
}

std::shared_ptr<Text> WidgetFactory::ReadText(const YAML::Node& asset)
{
    std::shared_ptr<Text> textWidget;
    
    vec3 color = ReadVec3(asset, "color");
    float scale = ReadFloat(asset, "scale", 1.f);
    string text = ReadString(asset, "text");
    vec2 pos = ReadVec2(asset, "position");
    
    if (asset["name"])
    {
        textWidget = std::make_shared<Text>(text, color, scale, pos, ReadString(asset, "name"));
    }
    else
    {
        textWidget = std::make_shared<Text>(text, color, scale, pos);
    }
    
    return textWidget;
}

std::shared_ptr<Panel> WidgetFactory::ReadPanel(const YAML::Node& asset)
{
    vec2 pos = ReadVec2(asset, "position");
    
    std::shared_ptr<Panel> widget;
    if (asset["name"])
    {
        widget = std::make_shared<Panel>(pos, ReadString(asset, "name"));
    }
    else
    {
        widget = std::make_shared<Panel>(pos);
    }
    return widget;
}



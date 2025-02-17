#include "WidgetFactory.h"

#include "Reader.h"
#include "Managers/World.h"
#include "UI/Image2D.h"
#include "UI/Text.h"
#include "UI/Buttons/LoadSceneButton.h"

using namespace Reader;
using namespace Reflection;

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
            std::shared_ptr<Image2D> widget = std::shared_ptr<Image2D>();
            deserialize(asset, widget);
            SaveWidget(widget, parent);
            return widget;
        });
    
    RegisterWidget("TEXT", [](const YAML::Node& asset, const std::shared_ptr<Widget>& parent) ->
        std::shared_ptr<Widget> {
            std::shared_ptr<Text> widget = std::shared_ptr<Text>();
            deserialize(asset, widget);
            widget->size = widget->CalculateSize();
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
    widget->SetAutoName();
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

std::shared_ptr<Button> WidgetFactory::ReadButton(const YAML::Node& asset)
{
    string subtype = ReadString(asset, "subtype");
    if (subtype == "Start")
    {
        std::shared_ptr<LoadSceneButton> button = std::make_shared<LoadSceneButton>();
        deserialize(asset, button);
        return button;
    }
    std::shared_ptr<Button> button = std::make_shared<Button>();
    deserialize(asset, button);
    return button;
}

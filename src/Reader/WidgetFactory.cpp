#include "WidgetFactory.h"

#include "Reader.h"
#include "Managers/World.h"
#include "UI/Image2D.h"
#include "UI/Text/Text.h"
#include "UI/Containers/Panel.h"
#include "UI/Buttons/LoadSceneButton.h"
#include "UI/Containers/EntityPanel.h"

using namespace Reader;
using namespace Reflection;

using WidgetCreator = std::function<std::shared_ptr<Widget> (const YAML::Node&, Widget* parent)>;

std::shared_ptr<WidgetFactory> WidgetFactory::instance = nullptr;

void WidgetFactory::WidgetFactorySetup()
{
    RegisterWidget("BUTTON", [this](const YAML::Node& asset, Widget* parent) ->
        std::shared_ptr<Widget>  {
            std::shared_ptr<Button> widget = ReadButton(asset);
            SaveWidget(widget, parent);
            std::shared_ptr<Widget> w = std::static_pointer_cast<Widget>(widget);
            // POR QUE NECESITO ESTA VUELTA???
            temp = w;
            return w;
        });
    
    RegisterWidget("IMAGE2D", [this](const YAML::Node& asset, Widget* parent) ->
        std::shared_ptr<Widget> {
            std::shared_ptr<Image2D> widget = std::shared_ptr<Image2D>();
            deserialize(asset, widget);
            SaveWidget(widget, parent);
            std::shared_ptr<Widget> w = std::static_pointer_cast<Widget>(widget);
            // POR QUE NECESITO ESTA VUELTA???
            temp = w;
            return w;
        });
    
    RegisterWidget("TEXT", [this](const YAML::Node& asset, Widget* parent) ->
        std::shared_ptr<Widget> {
            std::shared_ptr<Text> widget = std::shared_ptr<Text>();
            deserialize(asset, widget);
            SaveWidget(widget, parent);
            std::shared_ptr<Widget> w = std::static_pointer_cast<Widget>(widget);
            // POR QUE NECESITO ESTA VUELTA???
            temp = w;
            return w;
        });
    
    RegisterWidget("PANEL", [this](const YAML::Node& asset, Widget* parent) ->
        std::shared_ptr<Widget> {
            std::shared_ptr<Panel> widget = std::shared_ptr<Panel>();
            deserialize(asset, widget);
            SaveWidget(widget, parent);
            std::shared_ptr<Widget> w = std::static_pointer_cast<Widget>(widget);
            // POR QUE NECESITO ESTA VUELTA???
            temp = w;
            return w;
    });
}

void WidgetFactory::RegisterWidget(const std::string& type, WidgetCreator creator)
{
    creators[type] = std::move(creator);
}

std::shared_ptr<Widget> WidgetFactory::CreateWidget(const std::string& type, const YAML::Node& data, Widget* parent) const
{
    auto it = creators.find(type);
    if (it != creators.end()) {
        shared_ptr<Widget> a = it->second(data, parent);
        return temp;
    }
    throw std::runtime_error("Unknown entity type: " + type);
}

void WidgetFactory::SaveWidget(const std::shared_ptr<Widget>& widget, Widget* parent)
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

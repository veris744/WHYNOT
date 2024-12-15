#include "AssetReader.h"

#include "ComponentFactory.h"
#include "WidgetFactory.h"
#include "Components/Model.h"
#include "Managers/World.h"


void AssetReader::ReadAssets(const char* filePath)
{    
    YAML::Node fileYaml = YAML::LoadFile(filePath);
    if (fileYaml["assets"])
    {
        for (const YAML::Node& asset : fileYaml["assets"])
        {
            SaveEntity(asset);
        }
    }
    if (fileYaml["widgets"])
    {
        for (const YAML::Node& asset : fileYaml["widgets"])
        {
            SaveWidget(asset);
        }
    }
}

void AssetReader::SaveEntity(const YAML::Node& asset)
{
    std::shared_ptr<Entity> entity = std::make_shared<Entity>(asset["name"].as<std::string>(),
        asset["isCamera"].as<bool>(), asset["isLight"].as<bool>());
    entity->isRendered = asset["isRendered"].as<bool>();

    for (const YAML::Node& component : asset["components"])
    {
        SaveComponent(entity, component);
    }

    World::GetInstance()->AddEntity(entity);
}

void AssetReader::SaveComponent(const std::shared_ptr<Entity>& entity, const YAML::Node& asset)
{
    string name = asset["name"].as<std::string>();
    ComponentFactory::GetInstance()->CreateComponent(name, entity, asset);
}

void AssetReader::SaveWidget(const YAML::Node& asset, const std::shared_ptr<Widget>& parent)
{
    string type = asset["type"].as<std::string>();
    std::shared_ptr<Widget> widget = WidgetFactory::GetInstance()->CreateWidget(type, asset, parent);
    if (asset["widgets"])
    {
        for (const YAML::Node& widgetYAML : asset["widgets"])
        {
            SaveWidget(widgetYAML, widget);
        }
    }
}

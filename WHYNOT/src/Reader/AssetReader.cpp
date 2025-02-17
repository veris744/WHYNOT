#include "AssetReader.h"

#include "ComponentFactory.h"
#include "EntityFactory.h"
#include "WidgetFactory.h"
#include "Components/Model.h"
#include "Managers/World.h"


void AssetReader::ReadAssets(const char* filePath)
{    
    YAML::Node fileYaml = YAML::LoadFile(filePath);
    if (fileYaml["widgets"])
    {
        for (const YAML::Node& asset : fileYaml["widgets"])
        {
            SaveWidget(asset);
        }
    }
    if (fileYaml["entities"])
    {
        for (const YAML::Node& asset : fileYaml["entities"])
        {
            SaveEntity(asset);
        }
    }
}

void AssetReader::SaveEntity(const YAML::Node& asset)
{
    std::shared_ptr<Entity> entity;
    if (asset["type"])
    {
        string type = asset["type"].as<std::string>();
        std::transform(type.begin(), type.end(), type.begin(), toupper);
        entity = EntityFactory::GetInstance()->CreateEntity(type, asset);
        
    }
    else
    {
        entity = std::make_shared<Entity>();
        Reflection::deserialize(asset, entity);
    }
    if (asset["components"])
    {
        for (const YAML::Node& component : asset["components"])
        {
            SaveComponent(entity, component);
        }
    }
    entity->Initialize();
}

void AssetReader::SaveComponent(const std::shared_ptr<Entity>& entity, const YAML::Node& asset)
{
    string type = asset["name"].as<std::string>();
    ComponentFactory::GetInstance()->CreateComponent(type, entity, asset);
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

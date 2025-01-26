#include "EntityFactory.h"

#include <utility>

#include "Entities/Alien.h"
#include "Entities/Projectile.h"
#include "Managers/World.h"


using ComponentCreator = std::function<void (const YAML::Node&)>;

EntityFactory* EntityFactory::instance = nullptr;

void EntityFactory::EntityFactorySetup()
{
    RegisterEntity("ALIEN", [](const YAML::Node& data) ->
        void {
            std::shared_ptr<Alien> alien = ReadAlien(data);
            World::GetInstance()->AddEntity(alien);
        });
    
    RegisterEntity("PROJECTILE", [](const YAML::Node& data) ->
        void {
            std::shared_ptr<Projectile> proj = ReadProjectile(data);
            World::GetInstance()->AddEntity(proj);
        });
}

void EntityFactory::RegisterEntity(const std::string& type, EntityCreator creator)
{
    creators[type] = std::move(creator);
}

void EntityFactory::CreateEntity(const std::string& type, const YAML::Node& data) const
{
    auto it = creators.find(type);
    if (it != creators.end()) {
        return it->second(data);
    }
    Logger::Log(LogLevel::FatalError, "Unknown entity type: " + type);
}


std::shared_ptr<Alien> EntityFactory::ReadAlien(const YAML::Node& asset)
{
    std::shared_ptr<Alien> alien = std::make_shared<Alien>();
    alien->Initialize();
    SetTransform(alien, asset);
    return alien;
}

std::shared_ptr<Projectile> EntityFactory::ReadProjectile(const YAML::Node& asset)
{
    std::shared_ptr<Projectile> projectile = std::make_shared<Projectile>();
    projectile->Initialize();
    SetTransform(projectile, asset);
    return projectile;
}

void EntityFactory::SetTransform(const std::shared_ptr<Entity>& _entity, const YAML::Node& asset)
{
    std::shared_ptr<Transform> transform = _entity->GetComponent<Transform>();
    if (transform == nullptr)
    {
        Logger::Log(LogLevel::Error, "Entity does not have a transform");
    }
    if (asset["position"])
    {
        transform->v_position = vec3(asset["position"][0].as<float>(),
             asset["position"][1].as<float>(), asset["position"][2].as<float>());
    }
    if (asset["scale"])
    {
        transform->v_scale = vec3(asset["scale"][0].as<float>(),
            asset["scale"][1].as<float>(), asset["scale"][2].as<float>());
    }
    if (asset["rotation"])
    {
        transform->v_rotation.SetRotation(asset["rotation"][0].as<float>(),
            asset["rotation"][1].as<float>(), asset["rotation"][2].as<float>());
    }
}

#include "EntityFactory.h"

#include <utility>

#include "Reader.h"
#include "Components/Transform.h"
#include "Entities/Alien.h"
#include "Entities/Projectile.h"
#include "Entities/Player.h"
#include "Managers/World.h"

using namespace Reader;

using ComponentCreator = std::function<void (const YAML::Node&)>;

std::shared_ptr<EntityFactory> EntityFactory::instance = nullptr;

void EntityFactory::EntityFactorySetup()
{
    RegisterEntity("ALIEN", [](const YAML::Node& data) ->
        void {
            std::shared_ptr<Alien> alien = ReadAlien(data);
        });
    
    RegisterEntity("PROJECTILE", [](const YAML::Node& data) ->
        void {
            std::shared_ptr<Projectile> proj = ReadProjectile(data);
        });
    
    RegisterEntity("PLAYER", [](const YAML::Node& data) ->
        void {
            std::shared_ptr<Player> player = ReadPlayer(data);
            if (World::GetInstance()->GetPlayer())
            {
                Logger::Log<EntityFactory>(LogLevel::Error, "Player already exists");
            }
            World::GetInstance()->SetPlayer(player);
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
    alien->GetComponent<Transform>()->v_position = ReadVec3(asset, "position");
    return alien;
}

std::shared_ptr<Projectile> EntityFactory::ReadProjectile(const YAML::Node& asset)
{
    std::shared_ptr<Projectile> projectile = std::make_shared<Projectile>();
    projectile->Initialize();
    SetTransform(projectile, asset);
    return projectile;
}

std::shared_ptr<Player> EntityFactory::ReadPlayer(const YAML::Node& asset)
{
    std::shared_ptr<Player> player = std::make_shared<Player>();
    player->Initialize();
    SetTransform(player, asset);
    return player;
}

void EntityFactory::SetTransform(const std::shared_ptr<Entity>& _entity, const YAML::Node& asset)
{
    std::shared_ptr<Transform> transform = _entity->GetComponent<Transform>();
    if (transform == nullptr)
    {
        Logger::Log(LogLevel::Error, "Entity does not have a transform");
    }
    transform->v_position = ReadVec3(asset, "position");
    transform->v_scale = ReadVec3(asset, "scale", vec3(1.f, 1.f, 1.f));
    transform->v_rotation.SetRotation(ReadVec3(asset, "rotation"));
}

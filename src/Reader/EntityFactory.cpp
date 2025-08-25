#include "EntityFactory.h"

#include <utility>
#include <Entities/Thrower/Goal.h>

#include "Reader.h"
#include "Entities/Alien.h"
#include "Entities/Projectile.h"
#include "Entities/Player.h"
#include "Managers/World.h"

using namespace Reader;

using ComponentCreator = std::function<std::shared_ptr<Entity> (const YAML::Node&)>;
using namespace Reflection;

std::shared_ptr<EntityFactory> EntityFactory::instance = nullptr;

void EntityFactory::EntityFactorySetup()
{
    RegisterEntity("ALIEN", [](const YAML::Node& data) ->
        std::shared_ptr<Entity> {
            std::shared_ptr<Alien> alien = std::make_shared<Alien>();
            deserialize(data, alien);
            return alien;
        });
    
    RegisterEntity("PROJECTILE", [](const YAML::Node& data) ->
        std::shared_ptr<Entity> {
            std::shared_ptr<Projectile> proj = std::make_shared<Projectile>();
            deserialize(data, proj);
            return proj;
        });

    RegisterEntity("GOAL", [](const YAML::Node& data) ->
        std::shared_ptr<Entity> {
            std::shared_ptr<Goal> goal = std::make_shared<Goal>();
            deserialize(data, goal);
            return goal;
        });
}

void EntityFactory::RegisterEntity(const std::string& type, EntityCreator creator)
{
    creators[type] = std::move(creator);
}

std::shared_ptr<Entity> EntityFactory::CreateEntity(const std::string& type, const YAML::Node& data) const
{
    auto it = creators.find(type);
    if (it != creators.end()) {
        return it->second(data);
    }
    Logger::Log(LogLevel::FatalError, "Unknown entity type: " + type);
    return nullptr;
}

#pragma once
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

#ifndef YAML_CPP_STATIC_DEFINE
#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>
#endif

class Player;
class Projectile;
class Entity;
class Transform;
class Alien;

class EntityFactory
{
public:
    using EntityCreator = std::function<std::shared_ptr<Entity> (const YAML::Node&)>;

private:
    static std::shared_ptr<EntityFactory> instance;
    std::unordered_map<std::string, EntityCreator> creators;
    
public:
    EntityFactory() = default;
    ~EntityFactory() = default;

    static std::shared_ptr<EntityFactory> GetInstance()
    {
        if (instance == nullptr)
        {
            instance = std::make_shared<EntityFactory>(EntityFactory());
            instance->EntityFactorySetup();
        }
        return instance;
    }
    
    void EntityFactorySetup();
    void RegisterEntity(const std::string& type, EntityCreator creator);
    std::shared_ptr<Entity> CreateEntity(const std::string& type, const YAML::Node& data) const;
};

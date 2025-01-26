#pragma once
#include <functional>
#include <unordered_map>
#include <yaml-cpp/node/node.h>

class Projectile;
class Entity;
class Transform;
class Alien;

class EntityFactory
{
public:
    using EntityCreator = std::function<void (const YAML::Node&)>;

private:
    static EntityFactory* instance;
    std::unordered_map<std::string, EntityCreator> creators;
    
    static std::shared_ptr<Alien> ReadAlien(const YAML::Node& asset);
    static std::shared_ptr<Projectile> ReadProjectile(const YAML::Node& asset);

    static void SetTransform(const std::shared_ptr<Entity>& _entity, const YAML::Node& asset);
    
public:
    EntityFactory() = default;
    ~EntityFactory() = default;

    static EntityFactory* GetInstance()
    {
        if (instance == nullptr)
        {
            instance = new EntityFactory();
            instance->EntityFactorySetup();
        }
        return instance;
    }
    
    void EntityFactorySetup();
    void RegisterEntity(const std::string& type, EntityCreator creator);
    void CreateEntity(const std::string& type, const YAML::Node& data) const;
};

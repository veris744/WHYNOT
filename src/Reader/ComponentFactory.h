#pragma once
#include <functional>
#include <memory>

#ifndef YAML_CPP_STATIC_DEFINE
#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>
#endif

class Entity;
class CircleCollider;
class Collider;
class PlayerController;
class Material;
class LightSource;
class Camera;
class Model;
class Mesh;
class Transform;

class ComponentFactory
{
public:
    using ComponentCreator = std::function<void (const std::shared_ptr<Entity>& entity, const YAML::Node&)>;

private:
    static std::shared_ptr<ComponentFactory> instance;
    std::unordered_map<std::string, ComponentCreator> creators;
    
    static std::unique_ptr<Mesh> ReadMesh(const YAML::Node& asset);
    
public:
    ComponentFactory() = default;

    static std::shared_ptr<ComponentFactory> GetInstance()
    {
        if (instance == nullptr)
        {
            instance = std::make_shared<ComponentFactory>(ComponentFactory());
            instance->ComponentFactorySetup();
        }
        return instance;
    }
    
    void ComponentFactorySetup();
    void RegisterComponent(const std::string& type, ComponentCreator creator);
    void CreateComponent(const std::string& type, const std::shared_ptr<Entity>& entity, const YAML::Node& data) const;
};

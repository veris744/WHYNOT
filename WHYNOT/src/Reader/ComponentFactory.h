#pragma once
#include <includes.h>

#include "Entities/Entity.h"

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
    
    static std::shared_ptr<Transform> ReadTransform(const YAML::Node& asset);
    static std::shared_ptr<Mesh> ReadMesh(const YAML::Node& asset);
    static std::shared_ptr<Model> ReadModel(const YAML::Node& asset);
    static std::shared_ptr<Camera> ReadCamera(const YAML::Node& asset);
    static std::shared_ptr<LightSource> ReadLight(const YAML::Node& asset);
    static std::shared_ptr<Material> ReadMaterial(const YAML::Node& asset);
    static std::shared_ptr<PlayerController> ReadMoveComp(const YAML::Node& asset);
    static std::shared_ptr<CircleCollider> ReadCircleCollider(const YAML::Node& asset);
    
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

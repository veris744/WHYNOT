#pragma once

#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>

#include "Graphics/Material.h"

class Entity;
class Mesh;
class LightSource;
class Camera;
class Model;
class Transform;
class Component;

class AssetReader
{
    static void SaveEntity(const YAML::Node& asset);
    static void GetComponent(const std::shared_ptr<Entity>& entity, const YAML::Node& asset);
     
public:
    static void ReadAssets(const char* filePath);

    static std::shared_ptr<Transform> ReadTransform(const YAML::Node& asset);
    static std::shared_ptr<Mesh> ReadMesh(const YAML::Node& asset);
    static std::shared_ptr<Model> ReadModel(const YAML::Node& asset);
    static std::shared_ptr<Camera> ReadCamera(const YAML::Node& asset);
    static std::shared_ptr<LightSource> ReadLight(const YAML::Node& asset);
    static std::shared_ptr<Material> ReadMaterial(const YAML::Node& asset);
    
};

#pragma once

#ifndef YAML_CPP_STATIC_DEFINE
#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>
#endif

class Widget;
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
    static void SaveComponent(const std::shared_ptr<Entity>& entity, const YAML::Node& asset);
    
    static void SaveWidget(const YAML::Node& asset, Widget* parent = nullptr);
     
public:
    static void ReadAssets(const char* filePath);

    
};

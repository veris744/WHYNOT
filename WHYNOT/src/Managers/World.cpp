#include "World.h"

#include "Components/Camera.h"
#include "Components/LightSource.h"
#include "Components/Mesh.h"
#include "Components/Transform.h"
#include "UI/Button.h"
#include "UI/Image2D.h"
#include "Utils/AssetReader.h"

World* World::instance = nullptr;

World* World::GetInstance()
{
    if (!instance)
    {
        instance = new World();
    }
    return instance;
}

World::World()
{
    
}

void World::InitializeData()
{
    std::shared_ptr<Button> button = std::make_shared<Button>("assets/box.jpg", vec2(20,0), vec2(200,200));
    widgets.push_back(button);
}

void World::Initialize()
{
    AssetReader::ReadAssets("assets/cubes.yaml");
    InitializeData();
}

void World::Update()
{
    for (const auto& entity : entities)
    {
        entity.second->Update();
    }
}

void World::AddEntity(const std::shared_ptr<Entity>& _entity)
{
    if (entities.count(_entity->GetName()))
    {
        Logger::Log<World>(LogLevel::Warning, _entity->GetName() + " entity already exists");
    }
    entities[_entity->GetName()] = _entity;
    
    if (_entity->IsCamera())
    {
        cameras[_entity->GetName()] = _entity;
        if (!currentCameraComp)
        {
           SetCurrentCamera(_entity);
        }
    }
    if (_entity->IsLight())
    {
        std::shared_ptr<LightSource> light = _entity->GetComponent<LightSource>();
        lights[_entity->GetName()] = light;
    }
}

void World::SetCurrentCamera(const std::shared_ptr<Entity>& _camera)
{
    currentCamera = _camera;
    currentCameraComp = _camera->GetComponent<Camera>();
    currentCameraTransform = _camera->GetComponent<Transform>();
}

vector<LightData> World::GetLightDataList() const
{
    vector<LightData> list;
    list.reserve(lights.size());
    for (const auto& light : lights)
    {
        list.push_back(light.second->lightData);
    }
    return list;
}


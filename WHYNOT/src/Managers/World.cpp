#include "World.h"

#include "Components/Camera.h"
#include "Components/LightSource.h"
#include "Components/Mesh.h"
#include "Components/Model.h"
#include "Components/Transform.h"
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
    
}

void World::Initialize()
{
    AssetReader::ReadAssets("assets/assets.yaml");
    AssetReader::ReadAssets("assets/widgets.yaml");
    AssetReader::ReadAssets("assets/entities.yaml");
    InitializeData();
}

void World::Update(float deltaTime)
{
    for (const auto& entity : entities)
    {
        entity.second->Update(deltaTime);
    }
    for (const auto& entity : toBeDestroyed)
    {
        RemoveEntity(entity->GetName());
    }
    toBeDestroyed.clear();
}

void World::CheckCollisions()
{
    for (const auto& entity : entities)
    {
        if (entity.second->hasCollision)
        {
            // calculate collision
        }
    }
}

void World::MarkForDestruction(const string& _entityName)
{
    toBeDestroyed.push_back(entities.at(_entityName));
}

void World::RemoveEntity(const string& _entityName)
{
    std::shared_ptr<Entity> entity = entities.at(_entityName);
    if (entity->IsCamera())
    {
        cameras.erase(entity->GetName());
    }
    if (entity->IsLight())
    {
        lights.erase(entity->GetName());
    }
    entities.erase(entity->GetName());
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


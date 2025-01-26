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
        std::shared_ptr<Model> model = entity.second->GetComponent<Model>();
        if (model)
        {
            entity.second->GetComponent<Transform>()->SetRotation(entity.second->GetComponent<Transform>()->v_rotation.pitch,
                entity.second->GetComponent<Transform>()->v_rotation.yaw += 20*deltaTime, entity.second->GetComponent<Transform>()->v_rotation.roll);
        }
    }
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

// void World::AddEntity(const std::shared_ptr<Entity>& _entity)
// {
//     if (entities.count(_entity->GetName()))
//     {
//         Logger::Log<World>(LogLevel::Warning, _entity->GetName() + " entity already exists");
//     }
//     entities[_entity->GetName()] = _entity;
//     
//     if (_entity->IsCamera())
//     {
//         cameras[_entity->GetName()] = _entity;
//         if (!currentCameraComp)
//         {
//            SetCurrentCamera(_entity);
//         }
//     }
//     if (_entity->IsLight())
//     {
//         std::shared_ptr<LightSource> light = _entity->GetComponent<LightSource>();
//         lights[_entity->GetName()] = light;
//     }
// }

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


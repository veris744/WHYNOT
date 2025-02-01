#pragma once
#include <includes.h>

#include "Components/LightData.h"
#include "Components/LightSource.h"
#include "Components/Camera.h"
#include "Entities/Entity.h"
#include "Entities/Player.h"

class Widget;

class World
{
    static std::shared_ptr<World> instance;
    map<string, std::shared_ptr<Entity>> entities;
    vector<std::shared_ptr<Camera>> cameras;
    vector<std::shared_ptr<LightSource>> lights;
    std::shared_ptr<Player> playerEntity;

    std::vector<std::shared_ptr<Entity>> toBeDestroyed;

    vector<std::shared_ptr<Widget>> widgets;
    
    World();
    void InitializeData();

    
public:
    vec2 boundariesX = vec2(-10, 10);
    vec2 boundariesY = vec2(-10, 10);
    vec2 boundariesZ = vec2(-10, 10);
    
    unsigned int currentCameraIndex = 0;
    
    static std::shared_ptr<World> GetInstance();
    void Initialize();
    void Update(float deltaTime);
    void CheckCollisions();

    std::shared_ptr<Player> GetPlayer() const { return playerEntity; }
    void SetPlayer(const std::shared_ptr<Player>& player) { playerEntity = player; }
    
    template <typename T>
    void AddEntity(const std::shared_ptr<T>& _entity)
    {
        if (entities.count(_entity->GetName()))
        {
            Logger::Log<World>(LogLevel::Warning, _entity->GetName() + " entity already exists");
            return;
        }
        entities[_entity->GetName()] = _entity;
    
        if (_entity->IsCamera())
        {
            std::shared_ptr<Camera> camera = _entity->GetComponent<Camera>();
            cameras.push_back(camera);
        }
        if (_entity->IsLight())
        {
            std::shared_ptr<LightSource> light = _entity->GetComponent<LightSource>();
            lights.push_back(light);
        }
    }

    void MarkForDestruction(const string& _entityName);
private:
    void RemoveEntity(const string& _entityName);

public:
    unsigned int GetEntityCount() const { return entities.size(); }
    std::shared_ptr<Entity> GetEntity(const string& _name) const { return entities.at(_name); }
    map<string, std::shared_ptr<Entity>> GetEntities() const { return entities; }

    unsigned int GetCameraCount() const { return cameras.size(); }
    std::shared_ptr<Camera> GetCamera(unsigned int _index) const;
    std::shared_ptr<Camera> GetCurrentCamera() const;
    void SetCurrentCamera(unsigned int _index) { currentCameraIndex = _index; }
    void SetCurrentCamera(const string& _entityName);
    
    unsigned int GetLightCount() const { return lights.size(); }
    std::shared_ptr<LightSource> GetLightSource(unsigned int _index) const;
    vector<LightData> GetLightDataList() const;

    vector<std::shared_ptr<Widget>> GetWidgets() const { return widgets; }
    void AddWidget(const std::shared_ptr<Widget>& _widget)
    {
        widgets.push_back(_widget);
    }
};

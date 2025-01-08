#pragma once
#include <includes.h>

#include "Components/LightData.h"
#include "Components/Transform.h"
#include "Entities/Entity.h"

class Widget;
class LightSource;
class Camera;

class World
{
    static World* instance;
    map<string, std::shared_ptr<Entity>> entities;
    map<string, std::shared_ptr<Entity>> cameras;
    map<string, std::shared_ptr<LightSource>> lights;

    std::shared_ptr<Entity> currentCamera;
    std::shared_ptr<Camera> currentCameraComp;
    std::shared_ptr<Transform> currentCameraTransform;

    vector<std::shared_ptr<Widget>> widgets;
    
    World();
    void InitializeData();

    
public:
    
    vec2 boundariesX = vec2(-10, 10);
    vec2 boundariesY = vec2(-10, 10);
    vec2 boundariesZ = vec2(-10, 10);
    
    string currentCameraName = "MainCamera";
    
    static World* GetInstance();
    void Initialize();
    void Update(float deltaTime);

    void AddEntity(const std::shared_ptr<Entity>& _entity);

    unsigned int GetEntityCount() const { return entities.size(); }
    std::shared_ptr<Entity> GetEntity(const string& _name) const { return entities.at(_name); }
    map<string, std::shared_ptr<Entity>> GetEntities() const { return entities; }

    unsigned int GetCameraCount() const { return cameras.size(); }
    std::shared_ptr<Entity> GetCamera(const string& _name) const
    {
        if (!cameras.at(_name))
        {
            Logger::Log<World>(LogLevel::Error, "No camera found");
            return nullptr;
        }
        return cameras.at(_name);
    }
    const std::shared_ptr<Camera>& GetCurrentCameraComp() const { return currentCameraComp; }
    const std::shared_ptr<Transform>& GetCurrentCameraTrans() const
    {
        return GetCamera(currentCameraName)->GetComponent<Transform>();
    }
    void SetCurrentCamera(const std::shared_ptr<Entity>& _camera);
    
    unsigned int GetLightCount() const { return lights.size(); }
    std::shared_ptr<LightSource> GetLightSource(const string& _name) const { return lights.at(_name); }
    vector<LightData> GetLightDataList() const;

    vector<std::shared_ptr<Widget>> GetWidgets() const { return widgets; }
    void AddWidget(const std::shared_ptr<Widget>& _widget)
    {
        widgets.push_back(_widget);
    }
};

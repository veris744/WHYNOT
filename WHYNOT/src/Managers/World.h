#pragma once
#include <includes.h>

#include "Components/LightSource.h"
#include "Components/Camera.h"
#include "Entities/Entity.h"
#include "Entities/Player.h"

class Widget;

class World
{
    static World* instance;
    unordered_map<string, std::shared_ptr<Entity>> entities;
    vector<Camera*> cameras;
    vector<LightSource*> lights;
    std::shared_ptr<Player> playerEntity;

    set<std::shared_ptr<Entity>> toBeDestroyed;
    set<std::shared_ptr<Widget>> toBeDestroyedWidgets;

    vector<std::shared_ptr<Widget>> widgets;

    static bool isSceneLoaded;
    static string currentScene;
    
    World();
    
public:
    vec2 boundariesX = vec2(-10, 10);
    vec2 boundariesY = vec2(-10, 10);
    vec2 boundariesZ = vec2(-10, 10);
    
    unsigned int currentCameraIndex = 0;
    
    static World* GetInstance();
    void Initialize();
    void PrepareLoad();
    void Update(float deltaTime);
    void Clean();

    std::shared_ptr<Player> GetPlayer() const { return playerEntity; }
    void SetPlayer(const std::shared_ptr<Player>& player) { playerEntity = player; }
    
    template <typename T>
    void AddEntity(const std::shared_ptr<T>& _entity)
    {
        if (_entity->GetName().empty())
        {
            _entity->SetAutoName();
        }
        if (entities.count(_entity->GetName()) != 0)
        {
            Logger::Log<World>(LogLevel::Warning, _entity->GetName() + " entity already exists");
            _entity->SetAutoName();
        }
        entities[_entity->GetName()] = _entity;
    
        if (_entity->IsCamera())
        {
            Camera* camera = _entity->template GetComponent<Camera>();
            cameras.push_back(camera);
        }
        if (_entity->IsLight())
        {
            LightSource* light = _entity->template GetComponent<LightSource>();
            lights.push_back(light);
        }
    }

    void MarkForDestruction(const std::shared_ptr<Entity>& _entity);
    void MarkForDestruction(const std::shared_ptr<Widget>& _widget);
private:
    void DestroyAsset(const std::shared_ptr<Entity>& _entity);
    void DestroyAsset(const std::shared_ptr<Widget>& _widget);
    
    std::shared_ptr<Widget> FindWidget(const string& _name, const vector<std::shared_ptr<Widget>>& _widgets) const;

public:
    unsigned int GetEntityCount() const { return entities.size(); }
    std::shared_ptr<Entity> GetEntity(const string& _name) const { return entities.at(_name); }
    unordered_map<string, std::shared_ptr<Entity>> GetEntities() const { return entities; }

    unsigned int GetCameraCount() const { return cameras.size(); }
    Camera* GetCamera(unsigned int _index) const;
    Camera* GetCurrentCamera() const;
    void SetCurrentCamera(unsigned int _index) { currentCameraIndex = _index; }
    void SetCurrentCamera(const string& _entityName);
    
    unsigned int GetLightCount() const { return lights.size(); }
    LightSource* GetLightSource(unsigned int _index) const;
    vector<LightData> GetLightDataList() const;

    vector<std::shared_ptr<Widget>> GetWidgets() const { return widgets; }
    Widget* GetWidget(const string& _name) const;
    void AddWidget(const std::shared_ptr<Widget>& _widget);

    void LoadScene(const string& _sceneName);
    void DoLoad();
    void UnloadScene();

    static bool IsSceneLoaded() { return isSceneLoaded; }
    void EndApplication();

    // static void ShowLoading();
};

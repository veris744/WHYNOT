#pragma once

#include <GameManagers/GameManager.h>

#include "Components/LightSource.h"
#include "Components/Camera.h"
#include "Entities/Entity.h"

class Widget;

class World
{
    static World* instance;
    std::unordered_map<std::string, std::shared_ptr<Entity>> entities;
    std::vector<Camera*> cameras;
    std::vector<LightSource*> lights;
    // Player* playerEntity;
    std::unique_ptr<Entity> editorViewerEntity;

    std::set<std::shared_ptr<Entity>> toBeDestroyed;
    std::set<std::shared_ptr<Widget>> toBeDestroyedWidgets;

    std::vector<std::shared_ptr<Widget>> widgets;

    static std::unique_ptr<GameManager> gameManager;

    static bool isSceneLoaded;
    static std::string currentScene;

    static bool isPaused;
    
    World();
    
public:
    glm::vec2 boundariesX = glm::vec2(-10, 10);
    glm::vec2 boundariesY = glm::vec2(-10, 10);
    glm::vec2 boundariesZ = glm::vec2(-10, 10);
    
    unsigned int currentCameraIndex = 0;
    
    static World* GetInstance();
    void Initialize();
    void PrepareLoad();
    void Update(float deltaTime);
    void Clean();
    static void Pause() { isPaused = true; };
    static void Resume() { isPaused = false; };
    static bool IsPaused() { return isPaused; };
    static std::string GetCurrentScene() { return currentScene; }
    static GameManager* GetGameManager() { return gameManager.get(); }

    //
    // void SetPlayer(Player* _player)
    // {
    //     if (playerEntity != nullptr)
    //     {
    //         Logger::Log(LogLevel::Warning, "Player already set. Replacing it.");
    //         entities.erase("Player");
    //     }
    //     playerEntity = _player;
    // }
    // Player* GetPlayer() const { return playerEntity; }
    Entity* GetEditorPlayer() const { return editorViewerEntity.get(); }
    
    template <typename T>
    void AddEntity(const std::shared_ptr<T>& _entity)
    {
        if (entities.count(_entity->GetName()) != 0)
        {
            Logger::Log<World>(LogLevel::Warning, _entity->GetName() + " entity already exists");
            return;
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
    
    std::shared_ptr<Widget> FindWidget(const std::string& _name, const std::vector<std::shared_ptr<Widget>>& _widgets) const;

public:
    unsigned int GetEntityCount() const { return entities.size(); }
    std::shared_ptr<Entity> GetEntity(const std::string& _name) const { return entities.at(_name); }
    std::unordered_map<std::string, std::shared_ptr<Entity>> GetEntities() const { return entities; }

    unsigned int GetCameraCount() const { return cameras.size(); }
    Camera* GetCamera(unsigned int _index) const;
    Camera* GetCurrentCamera() const;
    void SetCurrentCamera(unsigned int _index) { currentCameraIndex = _index; }
    void SetCurrentCamera(const std::string& _entityName);
    
    unsigned int GetLightCount() const { return lights.size(); }
    LightSource* GetLightSource(unsigned int _index) const;
    std::vector<LightData> GetLightDataList() const;

    std::vector<std::shared_ptr<Widget>> GetWidgets() const { return widgets; }
    std::vector<std::shared_ptr<Widget>> GetWidgetsChildOf(const Widget* _widget = nullptr) const;
    Widget* GetWidget(const std::string& _name) const;
    void AddWidget(const std::shared_ptr<Widget>& _widget);

    void LoadScene(const std::string& _sceneName);
    void DoLoad();
    void UnloadScene();

    static bool IsSceneLoaded() { return isSceneLoaded; }
    void EndApplication();

    // static void ShowLoading();
};

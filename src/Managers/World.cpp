#include "World.h"

#include <thread>
#include <Components/PlayerController.h>
#include <Components/Transform.h>
#include <GameManagers/MapManager.h>
#include <Utils/Parser.h>

#include "ConfigurationValues.h"
#include "Renderer.h"
#include "Components/Camera.h"
#include "Components/LightSource.h"
#include "Editor/EditorMode.h"
#include "Graphics/Shader.h"
#include "Input/InputManager.h"
#include "GameManagers/AliensLogic.h"
#include "Physics/CollisionManager.h"
#include "UI/Widget.h"
#include "Reader/AssetReader.h"
#include "Utils/Timer.h"

World* World::instance = nullptr;
bool World::isSceneLoaded = false;
string World::currentScene = "";
std::unique_ptr<GameManager> World::gameManager = nullptr;

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

void World::Initialize()
{
    editorViewerEntity = std::make_unique<Entity>("EditorPlayer", true, false, false, false);
    editorViewerEntity->AddComponent(std::make_unique<Transform>());
    editorViewerEntity->AddComponent(std::make_unique<Camera>());
    std::unique_ptr<Movement> move = std::make_unique<Movement>();
    move->SkipWhenPause(false);
    editorViewerEntity->AddComponent(std::move(move));
    std::unique_ptr<PlayerController> pc = std::make_unique<PlayerController>();
    pc->SkipWhenPause(false);
    pc->EnterFreeMode(true);
    editorViewerEntity->AddComponent(std::move(pc));
    editorViewerEntity->isActive = false;
    cameras.push_back(editorViewerEntity->GetComponent<Camera>());

    LoadScene("MainMenu");
}

void World::PrepareLoad()
{
    if (!isSceneLoaded && entities.empty())
    {
        DoLoad();
        Logger::Log(LogLevel::Info, "SceneLoaded");
    }
}

void World::Update(float deltaTime)
{
    if (!isSceneLoaded) return;

    if (editorViewerEntity->isActive)
    {
        editorViewerEntity->Update(deltaTime);
    }

    for (const auto& entity : entities)
    {
        if (!entity.second->isActive)
        {
            continue;
        }
        entity.second->UpdateTrigger(deltaTime);
    }
    
}

void World::Clean()
{
    for (const auto& entity : toBeDestroyed)
    {
        DestroyAsset(entity);
    }
    toBeDestroyed.clear();
    for (const auto& widget : toBeDestroyedWidgets)
    {
        DestroyAsset(widget);
    }
    toBeDestroyedWidgets.clear();
}


void World::MarkForDestruction(const std::shared_ptr<Entity>& _entity)
{
    toBeDestroyed.insert(_entity);
}

void World::MarkForDestruction(const std::shared_ptr<Widget>& _widget)
{
    toBeDestroyedWidgets.insert(_widget);
}

void World::DestroyAsset(const std::shared_ptr<Entity>& _entity)
{
    if (_entity->IsCamera())
    {
        cameras.erase(ranges::find(cameras, _entity->GetComponent<Camera>()));
    }
    if (_entity->IsLight())
    {
        lights.erase(ranges::find(lights, _entity->GetComponent<LightSource>()));
    }
    entities.erase(_entity->GetName());
    _entity->ClearComponents();
}

void World::DestroyAsset(const std::shared_ptr<Widget>& _widget)
{
    if (_widget->GetParent())
    {
        _widget->GetParent()->RemoveChild(_widget);
    }
    else
    {
        if (const auto& it = ranges::find(widgets, _widget); it != widgets.end())
            widgets.erase(it);
    }
    _widget->ClearChildren();
}

Camera* World::GetCamera(unsigned int _index) const
{
    if (_index >= cameras.size())
    {
        Logger::Log<World>(LogLevel::Error, "No camera found");
        return nullptr;
    }
    return cameras[_index];
}

Camera* World::GetCurrentCamera() const
{
    return cameras.at(currentCameraIndex);
}

void World::SetCurrentCamera(const string& _entityName)
{
    int i = 0;
    for (const auto& camera : cameras)
    {
        if (camera->parent->GetName() == _entityName)
        {
            currentCameraIndex = i;
            return;
        }
        i++;
    }
    Logger::Log<World>(LogLevel::Error, "No camera found");
}

LightSource* World::GetLightSource(unsigned int _index) const
{
    if (_index >= lights.size())
    {
        Logger::Log<World>(LogLevel::Error, "No light found");
        return nullptr;
    }
    return lights[_index];
}

vector<LightData> World::GetLightDataList() const
{
    vector<LightData> list;
    list.reserve(lights.size());
    for (const auto& light : lights)
    {
        list.push_back(light->lightData);
    }
    return list;
}

vector<std::shared_ptr<Widget>> World::GetWidgetsChildOf(const Widget* _widget) const
{
    vector<std::shared_ptr<Widget>> result;

    vector<std::shared_ptr<Widget>> directChildren = _widget ? _widget->GetChildren() : GetWidgets();
    result.reserve(directChildren.size());
    result.insert(result.end(), directChildren.begin(), directChildren.end());

    for (const auto& child : directChildren)
    {
        vector<std::shared_ptr<Widget>> grandchildren = GetWidgetsChildOf(child.get());
        result.insert(result.end(), grandchildren.begin(), grandchildren.end());
    }

    return result;
}

Widget* World::GetWidget(const string& _name) const
{
    std::shared_ptr<Widget> widget = FindWidget(_name, widgets);
    if (!widget)
    {
        Logger::Log<World>(LogLevel::Error, "Widget " + _name + " not found");
    }
    return widget.get();
}

std::shared_ptr<Widget> World::FindWidget(const string& _name, const vector<std::shared_ptr<Widget>>& _widgets) const
{
    for (const auto& widget : _widgets)
    {
        if (widget->GetName() == _name)
        {
            return widget;
        }
        shared_ptr<Widget> foundWidget = FindWidget(_name, widget->GetChildren());
        if (foundWidget)
        {
            return foundWidget;
        }
    }
    return nullptr;
}


void World::AddWidget(const std::shared_ptr<Widget>& _widget)
{
    auto it = std::find_if(widgets.begin(), widgets.end(), [&](const std::shared_ptr<Widget>& widget) {
        return _widget->GetName() == widget->GetName();
    });
    if (it != widgets.end())
    {
        Logger::Log<World>(LogLevel::Error, "Widget " + _widget->GetName() + " already exists");
        return;
    }
    widgets.push_back(_widget);
    _widget->Initialize();
}


void World::LoadScene(const string& _sceneName)
{
    UnloadScene();
    currentScene = _sceneName;    
}

void World::DoLoad()
{
    CollisionManager::ClearRoot();
    
    string file = "assets/scenes/" + currentScene + ".yaml";
    AssetReader::ReadAssets(file.c_str());
    ConfigurationValues::ActiveGame = currentScene;

    if (currentScene == "Aliens")
    {
        gameManager = std::make_unique<AliensLogic>();
        Resume();
    }
    else if (currentScene == "MainMenu")
    {
        Helper::SetCursorVisible(true);
        ConfigurationValues::CanPlayerLook = false;
        ConfigurationValues::ArePhysicsActive = false;
        ConfigurationValues::CanPlayerMove = false;
        ConfigurationValues::IsEditorOpen = false;
        ConfigurationValues::IsUIActive = true;
        Pause();
    }
    else if (currentScene == "Editor")
    {
        gameManager = std::make_unique<MapManager>();
        Resume();
    }
    if (gameManager)
    {
        gameManager->PrepareGame();
        gameManager->StartGame();
    }
    playerEntity->GetComponent<Transform>()->position = gameManager ? gameManager->GetPlayerStart() : vec3(0,0,0);
    playerEntity->GetComponent<Transform>()->SetRotation(gameManager ? gameManager->GetPlayerStartRotation() : vec3(0,0,0));
    SetCurrentCamera("Player");

    if (!playerEntity)
    {
        Logger::Log(LogLevel::FatalError,"Failed to load playerEntity");
    }
    playerEntity->isActive = true;

    if (ConfigurationValues::ArePhysicsActive)
    {
        CollisionManager::PrepareOctree();
    }
    
    isSceneLoaded = true;
    Timer::StartTimer(0.2, &InputManager::EnableInput, true);
}

void World::UnloadScene()
{
    if (currentScene == "Aliens")
    {
        gameManager->EndGame();
    }

    Renderer::instance().textures_loaded;
    isSceneLoaded = false;
    InputManager::EnableInput(false);
    for (const auto& entity : entities)
    {
        entity.second->Destroy();
    }
    for (const auto& widget : widgets)
    {
        widget->Destroy();
    }
    currentCameraIndex = 0;
    InputManager::GetInstance()->Clear();
    for (const auto& shader : Renderer::instance().shaders_loaded)
    {
        shader->CleanUp();
    }
    CollisionManager::ClearOctree();
    EditorMode::ClearEditor();
    gameManager.reset();
    playerEntity = nullptr;
    InputManager::GetInstance()->Clear();
}

void World::EndApplication()
{
    UnloadScene();
    InputManager::EnableInput(false);
    Renderer::instance().CleanUp();
    glfwSetWindowShouldClose(Helper::GetWindow(), true);
}


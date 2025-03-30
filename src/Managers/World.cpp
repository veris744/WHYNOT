#include "World.h"

#include <thread>

#include "ConfigurationValues.h"
#include "Renderer.h"
#include "Components/Camera.h"
#include "Components/LightSource.h"
#include "Editor/EditorMode.h"
#include "Graphics/Shader.h"
#include "Input/InputManager.h"
#include "Minigame1/AliensLogic.h"
#include "Physics/CollisionManager.h"
#include "UI/Widget.h"
#include "Reader/AssetReader.h"
#include "Utils/Timer.h"

World* World::instance = nullptr;
bool World::isSceneLoaded = false;
string World::currentScene = "";

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

    for (const auto& entity : entities)
    {
        if (!entity.second->isActive)
        {
            continue;
        }
        entity.second->UpdateTrigger(deltaTime);
    }
    CollisionManager::CheckCollisions();
    
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
    auto it = ranges::find(widgets, _widget);
    if (it != widgets.end())
    {
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
        AliensLogic::GetInstance()->PrepareGame();
        AliensLogic::GetInstance()->StartGame();
        Helper::SetCursorVisible(false);
        ConfigurationValues::CanPlayerLook = true;
        ConfigurationValues::ArePhysicsActive = true;
        ConfigurationValues::CanPlayerMove = false;
        ConfigurationValues::IsEditorOpen = false;
        ConfigurationValues::IsUIActive = false;
    }
    else if (currentScene == "MainMenu")
    {
        Helper::SetCursorVisible(true);
        ConfigurationValues::CanPlayerLook = false;
        ConfigurationValues::ArePhysicsActive = false;
        ConfigurationValues::CanPlayerMove = false;
        ConfigurationValues::IsEditorOpen = false;
        ConfigurationValues::IsUIActive = true;
    }
    else if (currentScene == "Editor")
    {
        Helper::SetCursorVisible(true);
        ConfigurationValues::CanPlayerLook = false;
        ConfigurationValues::ArePhysicsActive = false;
        ConfigurationValues::CanPlayerMove = true;
        ConfigurationValues::IsEditorOpen = true;
        ConfigurationValues::IsUIActive = true;
    }

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
        AliensLogic::GetInstance()->StopGame();
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
    playerEntity = nullptr;
    currentCameraIndex = 0;
    InputManager::GetInstance()->Clear();
    for (const auto& shader : Renderer::instance().shaders_loaded)
    {
        shader->CleanUp();
    }
    CollisionManager::ClearOctree();
    EditorMode::ClearEditor();
}

void World::EndApplication()
{
    UnloadScene();
    InputManager::EnableInput(false);
    Renderer::instance().CleanUp();
    glfwSetWindowShouldClose(Helper::GetWindow(), true);
}


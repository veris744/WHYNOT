#include "World.h"

#include <thread>

#include "Renderer.h"
#include "Components/Camera.h"
#include "Components/Collider.h"
#include "Components/LightSource.h"
#include "Graphics/Shader.h"
#include "Input/InputManager.h"
#include "Minigame1/AliensLogic.h"
#include "UI/Widget.h"
#include "Reader/AssetReader.h"
#include "Utils/OctreeNode.h"
#include "Utils/Timer.h"

std::shared_ptr<World> World::instance = nullptr;
bool World::isSceneLoaded = false;
string World::currentScene = "";

std::shared_ptr<World> World::GetInstance()
{
    if (!instance)
    {
        instance = std::make_shared<World>(World());
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
    CheckCollisions();
}

void World::CheckCollisions()
{
    AABB worldBounds = {vec3(Helper::GetXBounds().x, Helper::GetYBounds().x, Helper::GetZBounds().x), 
        vec3(Helper::GetXBounds().y, Helper::GetYBounds().y, Helper::GetZBounds().y)};
    
    OctreeNode root(worldBounds);
    for (auto& [id, entity] : entities) {
        if (entity && entity->isActive && entity->HasCollision()) {
            root.Insert(entity);
        }
    }
    
    std::set<pair<std::shared_ptr<Entity>, std::shared_ptr<Entity>>> collisions;
    root.QueryCollisions(collisions);
    
    for (const auto& collisionPair  : collisions) {
        const auto& e1 = collisionPair.first;
        const auto& e2 = collisionPair.second;
        
        if (!e1->isActive || !e2->isActive) continue;
        
        auto c1 = e1->GetComponent<Collider>();
        auto c2 = e2->GetComponent<Collider>();
        if (c1->Collides(c2)) {
            vec3 normal = normalize(c1->GetWorldPosition() - c2->GetWorldPosition());
            c1->CollisionDelegate.Execute(e2, normal);
            c2->CollisionDelegate.Execute(e1, -normal);
        }
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

std::shared_ptr<Widget> World::GetWidget(const string& _name) const
{
    std::shared_ptr<Widget> widget = FindWidget(_name, widgets);
    if (!widget)
    {
        Logger::Log<World>(LogLevel::Error, "Widget " + _name + " not found");
    }
    return widget;
}

std::shared_ptr<Widget> World::FindWidget(const string& _name, const vector<std::shared_ptr<Widget>>& _widgets) const
{
    for (const auto& widget : _widgets)
    {
        if (widget->GetName() == _name)
        {
            return widget;
        }
        auto foundWidget = FindWidget(_name, widget->GetChildren());
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
}


void World::LoadScene(const string& _sceneName)
{
    UnloadScene();
    currentScene = _sceneName;    
}

void World::DoLoad()
{    
    string file = "assets/scenes/" + currentScene + ".yaml";
    AssetReader::ReadAssets(file.c_str());

    if (currentScene == "Aliens")
    {
        AliensLogic::GetInstance()->PrepareGame();
        AliensLogic::GetInstance()->StartGame();  
        InputManager::SetInputMode(InputMode::GameOnly);
        Helper::SetCursorVisible(false);
    }
    if (currentScene == "MainMenu")
    {
        InputManager::SetInputMode(InputMode::UIOnly);
        Helper::SetCursorVisible(true);
    }

    if (!playerEntity)
    {
        Logger::Log(LogLevel::FatalError,"Failed to load playerEntity");
    }
    playerEntity->isActive = true;
    
    isSceneLoaded = true;
    Timer::StartTimer(0.2, &InputManager::EnableInput, true);
}

void World::UnloadScene()
{
    Renderer::GetInstance()->textures_loaded;
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
    for (const auto& shader : Renderer::GetInstance()->shaders_loaded)
    {
        shader->CleanUp();
    }
}

void World::EndApplication()
{
    UnloadScene();
    InputManager::EnableInput(false);
    Renderer::GetInstance()->CleanUp();
    glfwSetWindowShouldClose(Helper::GetWindow(), true);
}


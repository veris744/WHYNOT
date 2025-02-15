#include "World.h"

#include <thread>

#include "Renderer.h"
#include "Components/Camera.h"
#include "Components/Collider.h"
#include "Components/LightSource.h"
#include "Input/InputManager.h"
#include "Minigame1/AliensLogic.h"
#include "UI/Widget.h"
#include "Reader/AssetReader.h"
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
    for (auto it = entities.begin(); it != entities.end(); it++)
    {
        if (!it->second || !it->second->isActive)
        {
            continue;
        }
        if (it->second->hasCollision)
        {
            for (auto it2 = std::next(it); it2 != entities.end(); it2++)
            {
                if (it2->second && it2->second->hasCollision && it2->second->isActive)
                {
                    std::shared_ptr<Collider> c1 = it->second->GetComponent<Collider>();
                    std::shared_ptr<Collider> c2 = it2->second->GetComponent<Collider>();
                    if (c1->Collides(c2))
                    {
                        c1->CollisionDelegate.Execute(it2->second, normalize(c1->GetWorldPosition() - c2->GetWorldPosition()));
                        c2->CollisionDelegate.Execute(it->second, normalize(c2->GetWorldPosition() - c1->GetWorldPosition()));
                    }
                }
                if (!it->second || !it->second->isActive)
                {
                    break;
                }
            }
        }
    }
}

void World::Clean()
{
    for (const auto& entity : toBeDestroyed)
    {
        RemoveEntity(entity->GetName());
    }
    toBeDestroyed.clear();
}

void World::MarkForDestruction(const string& _entityName)
{
    auto it = std::find_if(toBeDestroyed.begin(), toBeDestroyed.end(), [&](const std::shared_ptr<Entity>& entity) {
        return entity->GetName() == _entityName;
    });
    if (it == toBeDestroyed.end())
    {
        toBeDestroyed.push_back(entities.at(_entityName));
    }
}

void World::RemoveEntity(const string& _entityName)
{
    std::shared_ptr<Entity> entity = entities.at(_entityName);
    if (entity->IsCamera())
    {
        cameras.erase(std::find(cameras.begin(), cameras.end(), entity->GetComponent<Camera>()));
    }
    if (entity->IsLight())
    {
        lights.erase(std::find(lights.begin(), lights.end(), entity->GetComponent<LightSource>()));
    }
    entities.erase(entity->GetName());
    entity->ClearComponents();
}

std::shared_ptr<Camera> World::GetCamera(unsigned int _index) const
{
    if (_index >= cameras.size())
    {
        Logger::Log<World>(LogLevel::Error, "No camera found");
        return nullptr;
    }
    return cameras[_index];
}

std::shared_ptr<Camera> World::GetCurrentCamera() const
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

std::shared_ptr<LightSource> World::GetLightSource(unsigned int _index) const
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
    isSceneLoaded = false;
    InputManager::EnableInput(false);
    for (const auto& entity : entities)
    {
        entity.second->Destroy();
    }
    widgets.clear();
    playerEntity = nullptr;
    currentCameraIndex = 0;
    InputManager::GetInstance()->Clear();
}

void World::EndApplication()
{
    isSceneLoaded = false;
    InputManager::EnableInput(false);
    Renderer::GetInstance()->CleanUp();
    glfwSetWindowShouldClose(Helper::GetWindow(), true);
}


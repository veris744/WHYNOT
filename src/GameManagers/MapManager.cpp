//
// Created by Isa on 06/04/2025.
//

#include "MapManager.h"

#include <Components/Movement.h>
#include <Components/Transform.h>
#include <Managers/ConfigurationValues.h>
#include <Managers/Helper.h>
#include <Managers/World.h>

void MapManager::PrepareGame()
{
    Helper::SetCursorVisible(false);
    ConfigurationValues::CanPlayerLook = true;
    ConfigurationValues::ArePhysicsActive = true;
    ConfigurationValues::CanPlayerMove = true;
    ConfigurationValues::IsEditorOpen = false;
    ConfigurationValues::IsUIActive = false;

    playerStart = {0, 11, 0};
}

void MapManager::StartGame()
{
    World::Resume();
}

void MapManager::EndGame()
{
    player = nullptr;
}

void MapManager::SetPlayer()
{
    if (!player)
    {
        std::shared_ptr<Player> temp = std::make_shared<Player>("Player");
        player = temp.get();
        player->Initialize();
    }

    player->GetComponent<Movement>()->usesPhysics = true;
    player->GetPhysicsMaterial()->hasGravity = true;

    player->GetComponent<Transform>()->position = GetPlayerStart();
    player->GetComponent<Transform>()->SetRotation(GetPlayerStartRotation());

    World::GetInstance()->SetCurrentCamera("Player");

    player->isActive = true;
}

void MapManager::ProcessInput(int key, bool press)
{
}

void MapManager::Update(float deltaTime)
{
}

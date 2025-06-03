#include "InMenuManager.h"

#include <Components/Movement.h>
#include <Components/Transform.h>
#include <Managers/ConfigurationValues.h>
#include <Managers/Helper.h>
#include <Managers/World.h>

void InMenuManager::PrepareGame()
{
    Helper::SetCursorVisible(true);
    ConfigurationValues::CanPlayerLook = false;
    ConfigurationValues::ArePhysicsActive = false;
    ConfigurationValues::CanPlayerMove = false;
    ConfigurationValues::IsEditorOpen = false;
    ConfigurationValues::IsUIActive = true;

    SetPlayer();
}

void InMenuManager::StartGame()
{
    World::Pause();
}

void InMenuManager::EndGame()
{
    player = nullptr;
}

void InMenuManager::SetPlayer()
{
    if (!player)
    {
        std::shared_ptr<Player> temp = std::make_shared<Player>("Player");
        player = temp.get();
        player->Initialize();
    }

    player->GetComponent<Movement>()->usesPhysics = false;
    player->GetPhysicsMaterial()->hasGravity = false;

    player->GetComponent<Transform>()->position = GetPlayerStart();
    player->GetComponent<Transform>()->SetRotation(GetPlayerStartRotation());

    World::GetInstance()->SetCurrentCamera("Player");

    player->isActive = true;
}

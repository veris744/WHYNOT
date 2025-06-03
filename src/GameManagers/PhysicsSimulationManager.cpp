#include "PhysicsSimulationManager.h"

#include <Components/Movement.h>
#include <Components/Transform.h>
#include <Managers/ConfigurationValues.h>
#include <Managers/Helper.h>
#include <Managers/World.h>

void PhysicsSimulationManager::PrepareGame()
{
    Helper::SetCursorVisible(false);
    ConfigurationValues::CanPlayerLook = true;
    ConfigurationValues::ArePhysicsActive = true;
    ConfigurationValues::CanPlayerMove = true;
    ConfigurationValues::IsEditorOpen = false;
    ConfigurationValues::IsUIActive = false;

    playerStart = {0, 10, 5};
}

void PhysicsSimulationManager::StartGame()
{
    World::Resume();
}

void PhysicsSimulationManager::EndGame()
{
    player = nullptr;
}

void PhysicsSimulationManager::SetPlayer()
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

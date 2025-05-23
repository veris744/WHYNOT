#include "PhysicsSimulationManager.h"

#include <Components/Movement.h>
#include <Components/PlayerController.h>
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

    playerStart = {0, 15, 5};
    playerStartRotation = {-45, 0, 0};
}

void PhysicsSimulationManager::StartGame()
{
    World::GetInstance()->GetPlayer()->GetComponent<Movement>()->usesPhysics = false;
    World::GetInstance()->GetPlayer()->GetPhysicsMaterial()->hasGravity = false;
    World::GetInstance()->GetPlayer()->EnableCollisions(false);
}

void PhysicsSimulationManager::EndGame()
{
}

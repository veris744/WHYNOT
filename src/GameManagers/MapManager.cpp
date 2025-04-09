//
// Created by Isa on 06/04/2025.
//

#include "MapManager.h"

#include <Components/Movement.h>
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
    World::GetInstance()->GetPlayer()->GetComponent<Movement>()->usesPhysics = true;
    World::GetInstance()->GetPlayer()->GetComponent<Movement>()->isAffectedByGravity = true;
}

void MapManager::EndGame()
{
}

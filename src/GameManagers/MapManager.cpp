//
// Created by Isa on 06/04/2025.
//

#include "MapManager.h"

#include <Managers/ConfigurationValues.h>
#include <Managers/Helper.h>

void MapManager::PrepareGame()
{
    Helper::SetCursorVisible(false);
    ConfigurationValues::CanPlayerLook = true;
    ConfigurationValues::ArePhysicsActive = true;
    ConfigurationValues::CanPlayerMove = true;
    ConfigurationValues::IsEditorOpen = false;
    ConfigurationValues::IsUIActive = false;
}

void MapManager::StartGame()
{
}

void MapManager::EndGame()
{
}

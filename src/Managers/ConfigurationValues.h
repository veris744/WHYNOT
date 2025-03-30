#pragma once

#include <string>


struct ConfigurationValues
{
    static bool CanPlayerMove;
    static bool CanPlayerLook;

    static bool IsUIActive;
    static bool ArePhysicsActive;
    static bool IsEditorOpen;

    static std::string ActiveGame;
};
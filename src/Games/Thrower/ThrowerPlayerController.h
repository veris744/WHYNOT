#pragma once

#include "Components/PlayerController.h"

class ThrowerManager;

class ThrowerPlayerController : public PlayerController
{
    ThrowerManager* throwerManager = nullptr;

public:
    ThrowerPlayerController();

    void ProcessInput(int key, bool press) override;
};

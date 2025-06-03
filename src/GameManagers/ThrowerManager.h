#pragma once
#include "GameManager.h"

class ThrowerManager : public GameManager
{
public:
    ThrowerManager() = default;

    void PrepareGame() override;
    void StartGame() override;
    void EndGame() override;
    void SetPlayer() override;
};


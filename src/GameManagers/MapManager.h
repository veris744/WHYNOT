#pragma once
#include "GameManager.h"

class MapManager : public GameManager
{
public:
    MapManager() = default;

    void PrepareGame() override;
    void StartGame() override;
    void EndGame() override;
};


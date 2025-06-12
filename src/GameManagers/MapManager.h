#pragma once
#include "GameManager.h"

class MapManager : public GameManager
{
public:
    MapManager() = default;

    void PrepareGame() override;
    void StartGame() override;
    void EndGame() override;
    void SetPlayer() override;
    void ProcessInput(int key, bool press) override;
    void Update(float deltaTime) override;
};


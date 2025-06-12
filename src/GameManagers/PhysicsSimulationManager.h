#pragma once
#include "GameManager.h"

class PhysicsSimulationManager : public GameManager
{
public:
    PhysicsSimulationManager() = default;

    void PrepareGame() override;
    void StartGame() override;
    void EndGame() override;
    void SetPlayer() override;
    void ProcessInput(int key, bool press) override;
    void Update(float deltaTime) override;
};


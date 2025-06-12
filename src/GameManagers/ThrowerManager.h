#pragma once
#include <UI/Text/Text.h>

#include "GameManager.h"

class ProgressBar;
constexpr unsigned int TOTAL_BALLS = 5;

class ThrowerManager : public GameManager
{
    float maxPotency = 60;
    float minPotency = 1;
    bool isCharging = false;
    Entity* ballsReserve [TOTAL_BALLS];

    unsigned int score = 0;

public:
    ThrowerManager() = default;

    void PrepareGame() override;
    void StartGame() override;
    void EndGame() override;
    void SetPlayer() override;
    void ProcessInput(int key, bool press) override;
    void Update(float deltaTime) override;

    void AddPoints(unsigned int amount);


private:
    void PrepareUI();

    void GrabBall();
    void ReleaseBall();
    void ChargeBall();
    void ThrowBall();

    Entity* GenerateBall();

    Entity* GrabbedBall = nullptr;
    Transform* playerTransform = nullptr;
    Text* ballsCounter = nullptr;
    Text* pointsCounter = nullptr;
    ProgressBar* chargeBar = nullptr;

    unsigned int currentBall = 0;
};


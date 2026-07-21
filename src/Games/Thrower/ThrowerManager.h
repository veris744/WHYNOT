#pragma once
#include <UI/Text/Text.h>

#include "GameManagers/GameManager.h"

class ProgressBar;
constexpr unsigned int TOTAL_BALLS_HAND = 10;
constexpr unsigned int TOTAL_BALLS_SMALL = 4;
constexpr unsigned int TOTAL_BALLS_MEDIUM = 3;
constexpr unsigned int TOTAL_BALLS_LARGE = 2;
constexpr glm::vec3 BALLS_PLAYGROUND = glm::vec3(13,5,13);

enum class BallType
{
    SMALL, MEDIUM, LARGE, HAND
};

class ThrowerManager : public GameManager
{
    float maxPotency = 50;
    float minPotency = 1;
    bool isCharging = false;
    float forwardOffset = 7.f;
    Entity* ballsReserve [TOTAL_BALLS_HAND];

    unsigned int score = 0;

public:
    ThrowerManager() = default;

    void PrepareGame() override;
    void StartGame() override;
    void EndGame() override;
    void SetPlayer() override;
    void Update(float deltaTime) override;

    void AddPoints(unsigned int amount);

    void GrabBall();
    void ReleaseBall();
    void ChargeBall();
    void ThrowBall();


private:
    void PrepareUI();
    void PrepareScene();

    Entity* GenerateBall(BallType type, const std::string& name);

    Entity* GrabbedBall = nullptr;
    Transform* playerTransform = nullptr;
    Text* ballsCounter = nullptr;
    Text* pointsCounter = nullptr;
    ProgressBar* chargeBar = nullptr;

    unsigned int currentBall = 0;
};


#pragma once
#include "GameManager.h"

class InMenuManager : public GameManager
{
public:
    InMenuManager() = default;

    void PrepareGame() override;
    void StartGame() override;
    void EndGame() override;
    void SetPlayer() override;
};


#pragma once

#include "Entities/Player.h"

class ThrowerPlayer : public Player
{
public:

    ThrowerPlayer(const std::string& name = "") : Player(name) {}

    void Initialize() override;
};

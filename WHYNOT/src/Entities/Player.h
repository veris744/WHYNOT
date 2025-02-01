#pragma once
#include "Entity.h"

class Player : public Entity
{
    static unsigned int counter;
    
public:
    Player()
    {
        name = "Player" + std::to_string(++counter);
    }
    Player(const string& _name)
    {
        name = _name;
    }
    ~Player() override = default;

    void Initialize() override;
    void Update(float _deltaTime) override;
};

#pragma once
#include "Entity.h"

class Alien : public Entity
{
    static unsigned int counter;
    
public:
    Alien()
    {
        name = "Alien" + std::to_string(++counter);
    }
    explicit Alien(const string& _name)
    {
        name = _name;
    }
    ~Alien() override = default;

    void Initialize() override;
    void Update(float _deltaTime) override;
};

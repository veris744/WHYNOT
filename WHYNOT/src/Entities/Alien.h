#pragma once
#include "Entity.h"

class Alien : public Entity
{
    static unsigned int counter;

    void OnOutOfBounds(vec3 _normal);
    void OnCollision(const Entity& _otherEntity, vec3 normal);
    
public:
    Alien()
    {
        name = "Alien" + std::to_string(++counter);
    }
    Alien(const string& _name)
    {
        name = _name;
    }
    ~Alien() override = default;

    void Initialize() override;
    void Update(float _deltaTime) override;
};

#pragma once
#include "Entity.h"
#include "Reflection/Reflection.h"

class Movement;

class Alien : public Entity
{
    static unsigned int counter;

    void OnOutOfBounds(vec3 _normal);
    void OnCollision(const std::shared_ptr<Entity>& _otherEntity, vec3 normal);

    std::shared_ptr<Movement> movement;
    
public:
    Alien(const string& _name = "")
    {
        name = _name.empty() ? "Alien" + std::to_string(++counter) : _name;
    }
    ~Alien() override = default;

    void Initialize() override;
    void Update(float _deltaTime) override;
};
REGISTER_CLASS(Alien)
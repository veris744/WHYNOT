#pragma once
#include "Entity.h"

class Projectile : public Entity
{
    static unsigned int counter;

    void OnCollision(const Entity& _otherEntity, vec3 normal);
    void OnOutOfBounds(vec3 _normal);
    
public:
    Projectile()
    {
        name = "Projectile" + std::to_string(++counter);
    }
    Projectile(const string& _name)
    {
        name = _name;
    }
    ~Projectile() override = default;
    
    void Initialize() override;
    void Update(float _deltaTime) override;
};

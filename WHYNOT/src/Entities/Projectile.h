#pragma once
#include "Entity.h"
#include "Reflection/Reflection.h"


class Movement;

class Projectile : public Entity
{
    static unsigned int counter;

    std::shared_ptr<Movement> movementComp = nullptr;
    std::shared_ptr<Transform> transformComp = nullptr;
public:

    void OnCollision(const std::shared_ptr<Entity>& _otherEntity, vec3 normal);
    void OnOutOfBounds(vec3 _normal);
    
    Projectile(const string& _name = "")
    {
        name = _name.empty() ? "Projectile" + std::to_string(++counter) : _name;
    }
    ~Projectile() override = default;
    
    void Initialize() override;
    void Update(float _deltaTime) override;
    void GetShot();
    void DisableProjectile();
};
REGISTER_CLASS(Projectile);
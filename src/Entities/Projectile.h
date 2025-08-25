#pragma once
#include "Entity.h"
#include "Reflection/Reflection.h"


class AliensLogic;
class Movement;

class Projectile : public Entity
{
    static unsigned int counter;

    Movement* movementComp = nullptr;
    Transform* transformComp = nullptr;
    Transform* playerTransform = nullptr;
    AliensLogic* alienLogicManager = nullptr;
public:

    //void OnCollision(Entity* _otherEntity, vec3 normal);
    
    Projectile(const std::string& _name = "")
    {
        name = _name.empty() ? "Projectile" + std::to_string(++counter) : _name;
        //Logger::Log(LogLevel::Warning, "Projectile constructor: "   + name);
    }
    ~Projectile() override
    {        
        //Logger::Log(LogLevel::Warning, "Projectile destructor: " + name);
    };
    
    void Initialize() override;
    void Update(float _deltaTime) override;
    void ClearComponents() override;
    void GetShot();
    void DisableProjectile();
};
REGISTER_CLASS(Projectile, {
    REGISTER_MEMBER(Projectile, name, MemberProperty::Viewable)
});
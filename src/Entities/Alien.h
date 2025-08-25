#pragma once
#include "Entity.h"
#include "Reflection/Reflection.h"

class Movement;

class Alien : public Entity
{
    static unsigned int counter;

    //void OnOutOfBounds(vec3 _normal) override;
    void OnCollision(Entity* _otherEntity, glm::vec3 normal) override;

    Movement* movement = nullptr;
    Transform* transform = nullptr;
    Transform* playerTransform = nullptr;
    
public:
    Alien(const std::string& _name = "")
    {
        name = _name.empty() ? "Alien" + std::to_string(++counter) : _name;
    }
    ~Alien() override = default;

    void Initialize() override;
    void Update(float _deltaTime) override;
    void ClearComponents() override;
    
};
REGISTER_CLASS(Alien, {
    REGISTER_MEMBER(Alien, name, MemberProperty::Viewable)
});
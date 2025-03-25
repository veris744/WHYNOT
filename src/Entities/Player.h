#pragma once
#include "Entity.h"
#include "Reflection/Reflection.h"

class Player : public Entity
{
    static unsigned int counter;
    
public:
    Player(const string& _name = "")
    {
        name = _name.empty() ? "Player" + std::to_string(++counter) : _name;
    }
    ~Player() override = default;

    void Initialize() override;
    void Update(float _deltaTime) override;
};
REGISTER_CLASS(Player, {
    REGISTER_MEMBER(Player, name, MemberProperty::Viewable)
});

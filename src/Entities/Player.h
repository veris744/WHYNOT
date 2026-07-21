#pragma once
#include "Entity.h"
#include "Reflection/Reflection.h"

class PlayerController;

class Player : public Entity
{
    static unsigned int counter;
    
protected:
    PlayerController* playerController = nullptr;
    
public:
    Player(const std::string& _name = "")
    {
        name = _name.empty() ? "Player" + std::to_string(++counter) : _name;
    }
    ~Player() override = default;

    virtual void Initialize() override;
    virtual void Update(float _deltaTime) override;

    PlayerController* GetPlayerController() const { return playerController; }
};
REGISTER_CLASS(Player, {
    REGISTER_MEMBER(Player, name, MemberProperty::Viewable)
});

#pragma once

#include <queue>

#include "GameManager.h"
#include "UI/Text/Text.h"

class Projectile;
class Alien;

class AliensLogic : public GameManager
{
    std::vector<std::shared_ptr<Alien>> aliens;
    queue<std::shared_ptr<Projectile>> availableProjectiles;
    std::vector<std::shared_ptr<Projectile>> usedProjectiles;
    unsigned int totalAliens = 8;
    unsigned int totalProjectiles = 10;

    void CalculateRandomPosition(const std::shared_ptr<Alien>& alien);
    void CalculateRandomDirection(const std::shared_ptr<Alien>& alien);

    Text* alienText = nullptr;
    
    SingleDelegate<const string&> OnTextChangedDelegate;
    
public:
    AliensLogic() = default;
    void AlienDestroyed(const std::shared_ptr<Alien>& alien);

    void PrepareGame() override;
    void StartGame() override;
    void EndGame() override;

    void ShootProjectile();
    void RemoveProjectile(const std::shared_ptr<Projectile>& _projectile);
};

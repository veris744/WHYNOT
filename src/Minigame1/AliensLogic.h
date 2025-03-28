#pragma once

#include <queue>

#include "UI/Text/Text.h"

class Projectile;
class Alien;

class AliensLogic
{
    AliensLogic() = default;
    static std::shared_ptr<AliensLogic> instance;
    
    std::vector<std::shared_ptr<Alien>> aliens;
    queue<std::shared_ptr<Projectile>> availableProjectiles;
    std::vector<std::shared_ptr<Projectile>> usedProjectiles;
    unsigned int totalAliens = 10;
    unsigned int totalProjectiles = 10;

    vec3 playgroundDimensions = vec3(12,12,6);
    vec3 playgroundCenter = vec3(0,0,-8);
    
    void CalculateRandomPosition(const std::shared_ptr<Alien>& alien);
    void CalculateRandomDirection(const std::shared_ptr<Alien>& alien);

    Text* alienText = nullptr;
    
    SingleDelegate<const string&> OnTextChangedDelegate;
    
public:
    static std::shared_ptr<AliensLogic> GetInstance();

    vec2 GetXBounds() const;
    vec2 GetYBounds() const;
    vec2 GetZBounds() const;

    void AlienDestroyed(const std::shared_ptr<Alien>& alien);

    void PrepareGame(unsigned int _totalAliens = 0);
    void StartGame();
    void ShootProjectile();
    void RemoveProjectile(const std::shared_ptr<Projectile>& _projectile);
    void StopGame();
    
};

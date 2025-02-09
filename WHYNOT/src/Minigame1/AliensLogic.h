#pragma once
#include <includes.h>

#include "UI/Text.h"

class Projectile;
class Alien;

class AliensLogic
{
    AliensLogic() = default;
    static std::shared_ptr<AliensLogic> instance;
    
    std::vector<std::shared_ptr<Alien>> aliens;
    std::queue<std::shared_ptr<Projectile>> availableProjectiles;
    std::vector<std::shared_ptr<Projectile>> usedProjectiles;
    unsigned int totalAliens = 5;
    unsigned int totalProjectiles = 5;

    vec3 playgroundDimensions = vec3(15,15,15);
    vec3 playgroundCenter = vec3(0,0,6);
    

    void CalculateRandomPosition(const std::shared_ptr<Alien>& alien);
    void CalculateRandomDirection(const std::shared_ptr<Alien>& alien);

    std::shared_ptr<Text> alienText = nullptr;
    
public:
    static std::shared_ptr<AliensLogic> GetInstance();

    vec2 GetXBounds() const;
    vec2 GetYBounds() const;
    vec2 GetZBounds() const;

    void AlienDestroyed(const std::shared_ptr<Alien>& alien);

    void PrepareGame(unsigned int _totalAliens = 5);
    void StartGame();
    void ShootProjectile();
    void RemoveProjectile(const std::shared_ptr<Projectile>& _projectile);
    
};

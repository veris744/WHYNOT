#include "AliensLogic.h"

#include <random>

#include "Components/Audio.h"
#include "Components/CircleCollider.h"
#include "Components/Movement.h"
#include "Components/Transform.h"
#include "Entities/Alien.h"
#include "Entities/Projectile.h"
#include "Managers/AudioManager.h"
#include "Managers/World.h"

float getRandomFloat(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}

std::shared_ptr<AliensLogic> AliensLogic::instance = nullptr;

std::shared_ptr<AliensLogic> AliensLogic::GetInstance()
{
    if (instance == nullptr)
    {
        instance = std::make_shared<AliensLogic>(AliensLogic());
    }
    return instance;
}

vec2 AliensLogic::GetXBounds() const
{
    return vec2(playgroundCenter.x - playgroundDimensions.x * 0.5f, playgroundCenter.x + playgroundDimensions.x * 0.5f);
}

vec2 AliensLogic::GetYBounds() const
{
    return vec2(playgroundCenter.y - playgroundDimensions.y * 0.5f, playgroundCenter.y + playgroundDimensions.y * 0.5f);
}

vec2 AliensLogic::GetZBounds() const
{
    return vec2(playgroundCenter.z - playgroundDimensions.z * 0.5f, playgroundCenter.z + playgroundDimensions.z * 0.5f);
}

void AliensLogic::AlienDestroyed(const std::shared_ptr<Alien>& _alien)
{
    Logger::Log(LogLevel::Info, "AlienDestroying: " + _alien->GetName());
    aliens.erase(ranges::find(aliens, _alien));
    OnTextChangedDelegate.Execute("Aliens : " + std::to_string(aliens.size()));
    _alien->Destroy();
    if (aliens.empty())
    {
        StopGame();
    }
}

void AliensLogic::PrepareGame(unsigned int _totalAliens)
{
    totalAliens = _totalAliens <= 0 ? totalAliens : _totalAliens;
    for (unsigned int i = 0; i < totalAliens; i++)
    {
        std::shared_ptr<Alien> temp = std::make_shared<Alien>();
        temp->Initialize();
        temp->isActive = false;
        aliens.push_back(temp);
    }

    for (unsigned int i = 0; i < totalProjectiles; i++)
    {
        std::shared_ptr<Projectile> temp = std::make_shared<Projectile>();
        temp->Initialize();
        temp->isActive = false;
        availableProjectiles.push(temp);
    }
    alienText = std::static_pointer_cast<Text>(World::GetInstance()->GetWidget("AliensText"));
    alienText->isActive = true;
    OnTextChangedDelegate.Bind(&Text::SetText, alienText.get());
    OnTextChangedDelegate.Execute("Aliens : " + std::to_string(totalAliens));
    
}

void AliensLogic::StartGame()
{
    for (const auto& alien : aliens)
    {
        CalculateRandomPosition(alien);
        CalculateRandomDirection(alien);
        alien->isActive = true;
    }
}

void AliensLogic::ShootProjectile()
{
    if (availableProjectiles.empty())
    {
        World::GetInstance()->GetPlayer()->GetComponent<Audio>()->Play("NoShot");
        return;
    }
    usedProjectiles.push_back(availableProjectiles.front());
    availableProjectiles.pop();

    usedProjectiles.back()->GetShot();
    World::GetInstance()->GetPlayer()->GetComponent<Audio>()->Play("Shoot");
}

void AliensLogic::RemoveProjectile(const std::shared_ptr<Projectile>& _projectile)
{
    Logger::Log(LogLevel::Info, "ProjDestroying: " + _projectile->GetName());
    _projectile->DisableProjectile();
    
    if (usedProjectiles.empty() && availableProjectiles.empty()) return; // Game is stopping and data is being cleaned already
    
    usedProjectiles.erase(find(usedProjectiles.begin(), usedProjectiles.end(), _projectile));
    availableProjectiles.push(_projectile);
}

void AliensLogic::StopGame()
{
    aliens.clear();
    usedProjectiles.clear();
    while (!availableProjectiles.empty())
    {
        availableProjectiles.pop();
    }
    alienText = nullptr;

    World::GetInstance()->LoadScene("MainMenu");
}

void AliensLogic::CalculateRandomPosition(const std::shared_ptr<Alien>& alien)
{
    vec2 xBounds = GetXBounds();
    vec2 yBounds = GetYBounds();
    vec2 zBounds = GetZBounds();
    
    alien->GetComponent<Transform>()->position = vec3(getRandomFloat(xBounds.x, xBounds.y),
        getRandomFloat(yBounds.x, yBounds.y),
        getRandomFloat(zBounds.x, zBounds.y));

    for (const auto& otherAlien : aliens)
    {
        if (otherAlien == alien)    continue;
        if (!otherAlien->isActive)  continue;
        if (alien->GetComponent<Collider>()->Collides(otherAlien->GetComponent<Collider>()))
        {
            CalculateRandomPosition(otherAlien);
        }
    }
}

void AliensLogic::CalculateRandomDirection(const std::shared_ptr<Alien>& alien)
{
    vec3 dir = vec3(getRandomFloat(-1, 1), getRandomFloat(-1, 1), getRandomFloat(-1, 1));
    alien->GetComponent<Movement>()->speed = alien->GetComponent<Movement>()->maxSpeed * dir;
}
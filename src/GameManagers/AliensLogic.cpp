#include "AliensLogic.h"

#include <random>
#include <Components/PlayerController.h>
#include <Managers/ConfigurationValues.h>

#include "Components/Audio.h"
#include "Components/Colliders/CircleCollider.h"
#include "Components/Movement.h"
#include "Components/Transform.h"
#include "Entities/Alien.h"
#include "Entities/Projectile.h"
#include "Managers/Helper.h"
#include "Managers/World.h"
#include "Utils/Parser.h"

// float getRandomFloat(float min, float max) {
//     static std::random_device rd;
//     static std::mt19937 gen(rd());
//     std::uniform_real_distribution<float> dist(min, max);
//     return dist(gen);
// }

void AliensLogic::AlienDestroyed(const std::shared_ptr<Alien>& _alien)
{
    aliens.erase(ranges::find(aliens, _alien));
    OnTextChangedDelegate.Execute("Aliens : " + std::to_string(aliens.size()));
    _alien->Destroy();
    if (aliens.empty())
    {
        World::GetInstance()->LoadScene("MainMenu");
    }
}

void AliensLogic::PrepareGame()
{
    Helper::SetCursorVisible(false);
    ConfigurationValues::CanPlayerLook = true;
    ConfigurationValues::ArePhysicsActive = true;
    ConfigurationValues::CanPlayerMove = false;
    ConfigurationValues::IsEditorOpen = false;
    ConfigurationValues::IsUIActive = false;

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
    alienText = static_cast<Text*>(World::GetInstance()->GetWidget("AliensText"));
    alienText->isActive = true;
    OnTextChangedDelegate.Bind(&Text::SetText, alienText);
    OnTextChangedDelegate.Execute("Aliens : " + std::to_string(totalAliens));

    OnPlayerShootDelegate.Bind(&AliensLogic::ShootProjectile, this);
    playerStartRotation = {0, -45, 0};
    
}

void AliensLogic::StartGame()
{
    for (const auto& alien : aliens)
    {
        CalculateRandomPosition(alien);
        CalculateRandomDirection(alien);
        alien->isActive = true;
    }
    // World::GetInstance()->GetPlayer()->GetComponent<PlayerController>()->SetCanShoot(true);
    // World::GetInstance()->GetPlayer()->GetComponent<Movement>()->usesPhysics = false;
    // World::GetInstance()->GetPlayer()->GetPhysicsMaterial()->hasGravity = false;
}

void AliensLogic::ShootProjectile()
{
    // if (availableProjectiles.empty())
    // {
    //     World::GetInstance()->GetPlayer()->GetComponent<Audio>()->Play("NoShot");
    //     return;
    // }
    // usedProjectiles.push_back(availableProjectiles.front());
    // availableProjectiles.pop();
    //
    // usedProjectiles.back()->GetShot();
    // World::GetInstance()->GetPlayer()->GetComponent<Audio>()->Play("Shoot");
}

void AliensLogic::RemoveProjectile(const std::shared_ptr<Projectile>& _projectile)
{
    _projectile->DisableProjectile();
    
    if (usedProjectiles.empty() && availableProjectiles.empty()) return; // Game is stopping and data is being cleaned already
    
    usedProjectiles.erase(find(usedProjectiles.begin(), usedProjectiles.end(), _projectile));
    availableProjectiles.push(_projectile);
}

void AliensLogic::EndGame()
{
    for (const auto& alien : aliens)
    {
        alien->Destroy();
    }
    aliens.clear();
    for (const auto& usedProjectile : usedProjectiles)
    {
        usedProjectile->Destroy();
    }
    usedProjectiles.clear();
    while (!availableProjectiles.empty())
    {
        availableProjectiles.front()->Destroy();
        availableProjectiles.pop();
    }
    alienText = nullptr;
    // World::GetInstance()->GetPlayer()->GetComponent<PlayerController>()->SetCanShoot(false);
}

void AliensLogic::ProcessInput(int key, bool press)
{
}

void AliensLogic::Update(float deltaTime)
{
}

void AliensLogic::CalculateRandomPosition(const std::shared_ptr<Alien>& alien)
{
    vec2 xBounds = GetXBounds();
    vec2 yBounds = GetYBounds();
    vec2 zBounds = GetZBounds();

    float radius = alien->GetComponent<CircleCollider>()->radius;
    //
    // alien->GetComponent<Transform>()->position = vec3(getRandomFloat(xBounds.x+radius, xBounds.y-radius),
    //     getRandomFloat(yBounds.x+radius, yBounds.y-radius),
    //     getRandomFloat(zBounds.x+radius, zBounds.y-radius));

    for (const auto& otherAlien : aliens)
    {
        if (otherAlien == alien)    continue;
        if (!otherAlien->isActive)  continue;
        Hit hit;
        alien->GetComponent<Collider>()->Collides(otherAlien->GetComponent<Collider>(), hit);
        if (hit.hasHit)
        {
            CalculateRandomPosition(otherAlien);
        }
    }
}

void AliensLogic::CalculateRandomDirection(const std::shared_ptr<Alien>& alien)
{
    //vec3 dir = vec3(getRandomFloat(-1, 1), getRandomFloat(-1, 1), getRandomFloat(-1, 1));
    //alien->GetComponent<Movement>()->speed = alien->GetComponent<Movement>()->maxSpeed * dir;
}
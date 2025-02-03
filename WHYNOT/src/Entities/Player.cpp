#include "Player.h"

#include "Components/Camera.h"
#include "Components/PlayerController.h"
#include "Components/Transform.h"

unsigned int Player::counter = 0;

void Player::Initialize()
{
    isCamera = true;
    isLight = false;
    isRendered = false;
    hasCollision = false;

    std::shared_ptr<Camera> camera = std::make_shared<Camera>();
    AddComponent(camera);

    std::shared_ptr<Transform> transform = std::make_shared<Transform>(vec3(0));
    AddComponent(transform);

    std::shared_ptr<PlayerController> controller = std::make_shared<PlayerController>();
    controller->SetPositionLocked(false);
    AddComponent(controller);
    
    Entity::Initialize();
}

void Player::Update(float _deltaTime)
{
    Entity::Update(_deltaTime);
}

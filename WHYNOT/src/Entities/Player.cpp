#include "Player.h"

#include "Components/Audio.h"
#include "Components/Camera.h"
#include "Components/PlayerController.h"
#include "Components/Transform.h"
#include "Managers/World.h"

unsigned int Player::counter = 0;

void Player::Initialize()
{
    isCamera = true;
    isLight = false;
    isRendered = false;
    hasCollision = false;

    if (!GetComponent<Camera>())
    {
        std::shared_ptr<Camera> camera = std::make_shared<Camera>();
        AddComponent(camera);
    }

    if (!GetComponent<Transform>())
    {
        std::shared_ptr<Transform> transform = std::make_shared<Transform>(vec3(0));
        AddComponent(transform);
    }

    if (!GetComponent<PlayerController>())
    {
        std::shared_ptr<PlayerController> controller = std::make_shared<PlayerController>();
        controller->SetPositionLocked(false);
        AddComponent(controller);
    }

    if (!GetComponent<Audio>())
    {
        std::shared_ptr<Audio> audioSource = std::make_shared<Audio>();
        audioSource->AddAudioSource(AudioSource("Shoot", "assets/sounds/shoot.wav"));
        audioSource->AddAudioSource(AudioSource("NoShot", "assets/sounds/noshot.wav"));
        AddComponent(audioSource);
    }
    
    World::GetInstance()->SetPlayer(std::static_pointer_cast<Player>(shared_from_this()));
    Entity::Initialize();
}

void Player::Update(float _deltaTime)
{
    Entity::Update(_deltaTime);
}

#include "Player.h"

#include "Components/Audio.h"
#include "Components/Camera.h"
#include "Components/PlayerController.h"
#include "Components/Transform.h"
#include "Components/Movement.h"
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
        std::unique_ptr<Camera> camera = std::make_unique<Camera>();
        AddComponent(std::move(camera));
    }

    if (!GetComponent<Transform>())
    {
        std::unique_ptr<Transform> transform = std::make_unique<Transform>(vec3(0));
        AddComponent(std::move(transform));
    }

    if (!GetComponent<PlayerController>())
    {
        std::unique_ptr<PlayerController> controller = std::make_unique<PlayerController>();
        AddComponent(std::move(controller));
    }

    if (!GetComponent<Movement>())
    {
        std::unique_ptr<Movement> movement = std::make_unique<Movement>();
        movement->maxSpeed = 7;
        AddComponent(std::move(movement));
    }

    if (!GetComponent<Audio>())
    {
        std::unique_ptr<Audio> audioSource = std::make_unique<Audio>();
        audioSource->AddAudioSource(AudioSource("Shoot", "assets/sounds/shoot.wav"));
        audioSource->AddAudioSource(AudioSource("NoShot", "assets/sounds/noshot.wav"));
        AddComponent(std::move(audioSource));
    }

    isActive = true;
    // Entity::Initialize();
}

void Player::Update(float _deltaTime)
{
    Entity::Update(_deltaTime);
}

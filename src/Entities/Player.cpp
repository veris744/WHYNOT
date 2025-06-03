#include "Player.h"

#include <Components/Colliders/CapsuleCollider.h>
#include <Utils/Parser.h>

#include "Components/Audio.h"
#include "Components/Camera.h"
#include "Components/PlayerController.h"
#include "Components/Transform.h"
#include "Components/Movement.h"
#include "Managers/World.h"

unsigned int Player::counter = 0;

void Player::Initialize()
{
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
        movement->usesPhysics = true;
        movement->maxSpeed = 20;
        AddComponent(std::move(movement));
    }

    if (!GetComponent<PhysicsMaterial>())
    {
        std::unique_ptr<PhysicsMaterial> physMat = std::make_unique<PhysicsMaterial>();
        physMat->hasGravity = true;
        physMat->bounciness = 0.f;
        physMat->friction = 0.f;
        AddComponent(std::move(physMat));
    }

    if (!GetComponent<Audio>())
    {
        std::unique_ptr<Audio> audioSource = std::make_unique<Audio>();
        audioSource->AddAudioSource(AudioSource("Shoot", "assets/sounds/shoot.wav"));
        audioSource->AddAudioSource(AudioSource("NoShot", "assets/sounds/noshot.wav"));
        AddComponent(std::move(audioSource));
    }

    if (!GetComponent<Collider>())
    {
        std::unique_ptr<CapsuleCollider> collider = std::make_unique<CapsuleCollider>();
        collider->radius = 4;
        collider->height = 16;
        collider->profile = CollisionProfile(ColliderType::Dynamic, ColliderMode::All, false);
        AddComponent(std::move(collider));
    }

    isActive = true;
    Entity::Initialize();
}

void Player::Update(float _deltaTime)
{
    Entity::Update(_deltaTime);
    //Logger::Log(LogLevel::Info, "Player::Update: " + Parser::Parse(GetComponent<Transform>()->position));
}

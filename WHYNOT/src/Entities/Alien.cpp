#include "Alien.h"

#include "Components/CircleCollider.h"
#include "Components/Model.h"
#include "Components/Movement.h"
#include "Components/Transform.h"
#include "Managers/World.h"

unsigned int Alien::counter = 0;

void Alien::Initialize()
{
    Entity::Initialize();

    isCamera = false;
    isLight = false;
    isRendered = true;
    hasCollision = true;

    std::shared_ptr<Transform> transform = std::make_shared<Transform>();
    transform->v_scale = vec3(0.5f, 0.5f, 0.5f);
    AddComponent(transform);

    std::shared_ptr<Model> model = std::make_shared<Model>("assets/ufo/PinkAlien.obj");
    model->position = vec3(-0.5f, -0.5f, -0.5f);
    AddComponent(model);

    std::shared_ptr<CircleCollider> collider = std::make_shared<CircleCollider>(2.3);
    AddComponent(collider);

    std::shared_ptr<Movement> movement = std::make_shared<Movement>();
    AddComponent(movement);
}

void Alien::Update(float _deltaTime)
{
    Entity::Update(_deltaTime);
}

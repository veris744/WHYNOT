#include "Alien.h"

#include "Components/CircleCollider.h"
#include "Graphics/Mesh.h"
#include "Components/Model.h"
#include "Components/Movement.h"
#include "Components/Transform.h"
#include "Graphics/Material.h"
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

    std::shared_ptr<Material> mat = std::make_shared<Material>("", "", "shaders/fragmentColor.glsl");
    mat->materialData.type = MaterialType::COLOR;
    std::shared_ptr<Model> model = std::make_shared<Model>("assets/ufo/PinkAlien.obj", mat);
    model->position = vec3(-0.5f, -0.5f, -0.5f);
    AddComponent(model);

    std::shared_ptr<CircleCollider> collider = std::make_shared<CircleCollider>(2.3);
    AddComponent(collider);

    collider->OnOutOfBoundsDelegate.Bind(&Alien::OnOutOfBounds, this);

    std::shared_ptr<Movement> movement = std::make_shared<Movement>();
    movement->speed = vec3(3, 2, 2.5);
    AddComponent(movement);
    
    collider->CollisionDelegate.Bind(&Alien::OnCollision, this);
}

void Alien::Update(float _deltaTime)
{
    std::shared_ptr<CircleCollider> collider = GetComponent<CircleCollider>();
    collider->CheckInBounds(vec2(-8, 8), vec2(-4, 12), vec2(5, 16));
    
    GetComponent<Transform>()->SetRotation(GetComponent<Transform>()->v_rotation.pitch,
    GetComponent<Transform>()->v_rotation.yaw += 20*_deltaTime,GetComponent<Transform>()->v_rotation.roll);
    
    
    Entity::Update(_deltaTime);
}


void Alien::OnOutOfBounds(vec3 _normal)
{
    std::shared_ptr<Movement> movement = GetComponent<Movement>();

    float velocityTowardPlane = dot(_normal, movement->speed);
    if (velocityTowardPlane > 0)
    {
        return;
    }
    
    if (_normal.x != 0.f)
    {
        movement->speed.x = movement->speed.x * -1;
    }
    if (_normal.y != 0.f)
    {
        movement->speed.y = movement->speed.y * -1;
    }
    if (_normal.z != 0.f)
    {
        movement->speed.z = movement->speed.z * -1;
    }
}

void Alien::OnCollision(const Entity& _otherEntity, vec3 normal)
{
    Destroy();
}

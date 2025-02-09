#include "Alien.h"

#include "Projectile.h"
#include "Components/CircleCollider.h"
#include "Graphics/Mesh.h"
#include "Components/Model.h"
#include "Components/Movement.h"
#include "Components/Transform.h"
#include "Graphics/Material.h"
#include "Minigame1/AliensLogic.h"

unsigned int Alien::counter = 0;


void Alien::Initialize()
{
    isCamera = false;
    isLight = false;
    isRendered = true;
    hasCollision = true;
    isActive = false;

    std::shared_ptr<Transform> transform = std::make_shared<Transform>();
    transform->v_scale = vec3(0.4f, 0.4f, 0.4f);
    AddComponent(transform);

    std::shared_ptr<Material> mat = std::make_shared<Material>("", "", "shaders/fragmentColor.glsl");
    mat->materialData.type = MaterialType::COLOR;
    std::shared_ptr<Model> model = std::make_shared<Model>("assets/ufo/PinkAlien.obj", mat);
    model->position = vec3(-0.5f, -0.5f, -0.5f);
    AddComponent(model);

    std::shared_ptr<CircleCollider> collider = std::make_shared<CircleCollider>(1);
    AddComponent(collider);

    collider->OnOutOfBoundsDelegate.Bind(&Alien::OnOutOfBounds, this);

    movement = std::make_shared<Movement>();
    movement->maxSpeed = 5.0f;
    AddComponent(movement);
    
    collider->CollisionDelegate.Bind(&Alien::OnCollision, this);
    
    Entity::Initialize();
}

void Alien::Update(float _deltaTime)
{
    Entity::Update(_deltaTime);
    
    std::shared_ptr<CircleCollider> collider = GetComponent<CircleCollider>();
    collider->CheckInBounds(AliensLogic::GetInstance()->GetXBounds(),
        AliensLogic::GetInstance()->GetYBounds(),
        AliensLogic::GetInstance()->GetZBounds());
    
    GetComponent<Transform>()->SetRotation(GetComponent<Transform>()->v_rotation.pitch,
    GetComponent<Transform>()->v_rotation.yaw += 20*_deltaTime,GetComponent<Transform>()->v_rotation.roll);
}


void Alien::OnOutOfBounds(vec3 _normal)
{
    if (!isActive)  return;

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

void Alien::OnCollision(const std::shared_ptr<Entity>& _otherEntity, vec3 _normal)
{
    if (std::dynamic_pointer_cast<Projectile>(_otherEntity))
    {
        std::shared_ptr<Alien> self = std::static_pointer_cast<Alien>(shared_from_this());
        AliensLogic::GetInstance()->AlienDestroyed(self);
        Destroy();
        return;
    }
    
    float velocityTowardPlane = dot(_normal, movement->speed);
    if (velocityTowardPlane > 0)
    {
        return;
    }
    if (std::dynamic_pointer_cast<Alien>(_otherEntity))
    {
        movement->speed = _normal * length(movement->speed);
    }
}

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

    if (!GetComponent<Transform>())
    {
        std::shared_ptr<Transform> transform = std::make_shared<Transform>();
        transform->scale = vec3(0.3f, 0.3f, 0.3f);
        AddComponent(transform);
    }

    if (!GetComponent<Model>())
    {
        std::shared_ptr<Material> mat = std::make_shared<Material>("", DEFAULT_VERTEX_SHADER_PATH, "shaders/fragmentColor.glsl");
        mat->materialData.type = MaterialType::COLOR;
        std::shared_ptr<Model> model = std::make_shared<Model>("assets/ufo/PinkAlien.obj", mat);
        model->position = vec3(-0.3f, -0.3f, -0.35f);
        AddComponent(model);
    }

    
    if (!GetComponent<CircleCollider>())
    {
        std::shared_ptr<CircleCollider> collider = std::make_shared<CircleCollider>(0.65);
        AddComponent(collider);
    }
    
    GetComponent<CircleCollider>()->OnOutOfBoundsDelegate.Bind(&Alien::OnOutOfBounds, this);
    GetComponent<CircleCollider>()->CollisionDelegate.Bind(&Alien::OnCollision, this);

    if (!GetComponent<Movement>())
    {
        movement = std::make_shared<Movement>();
        movement->maxSpeed = 4.0f;
        AddComponent(movement);
    }
    else
    {
        movement = GetComponent<Movement>();
    }
    
    Entity::Initialize();
}

void Alien::Update(float _deltaTime)
{
    Entity::Update(_deltaTime);
    
    std::shared_ptr<CircleCollider> collider = GetComponent<CircleCollider>();
    collider->CheckInBounds(AliensLogic::GetInstance()->GetXBounds(),
        AliensLogic::GetInstance()->GetYBounds(),
        AliensLogic::GetInstance()->GetZBounds());
    
    GetComponent<Transform>()->SetRotation(GetComponent<Transform>()->rotation.pitch,
    GetComponent<Transform>()->rotation.yaw += 20*_deltaTime,GetComponent<Transform>()->rotation.roll);
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

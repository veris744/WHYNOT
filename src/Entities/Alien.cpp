#include "Alien.h"

#include <Utils/Parser.h>

#include "Projectile.h"
#include "Components/Colliders/CircleCollider.h"
#include "Graphics/Mesh.h"
#include "Components/Model.h"
#include "Components/Movement.h"
#include "Components/Transform.h"
#include "Graphics/Material.h"
#include "Managers/World.h"
#include "GameManagers/AliensLogic.h"

unsigned int Alien::counter = 0;


void Alien::Initialize()
{
    isCamera = false;
    isLight = false;
    isRendered = true;
    hasCollision = true;
    isActive = false;
    debugEnabled = false;

    if (!GetComponent<Transform>())
    {
        std::unique_ptr<Transform> transformPtr = std::make_unique<Transform>();
        transform = transformPtr.get();
        AddComponent(std::move(transformPtr));
    }
    else
    {
        transform = GetComponent<Transform>();
    }

    if (!GetComponent<Model>())
    {
        std::shared_ptr<Material> mat = std::make_shared<Material>("", DEFAULT_VERTEX_SHADER_PATH, "shaders/fragmentColor.glsl");
        mat->materialData.type = MaterialType::COLOR;
        std::unique_ptr<Model> model = std::make_unique<Model>("assets/ufo/PinkAlien.obj", mat);
        model->position = vec3(-0.5, -0.45, -0.5);
        model->rotation = vec3(0, -45, 0);
        model->scale = vec3(0.4, 0.4, 0.4);
        AddComponent(std::move(model));
    }

    
    if (!GetComponent<CircleCollider>())
    {
        std::unique_ptr<CircleCollider> collider = std::make_unique<CircleCollider>(0.85f);
        collider->profile = {ColliderType::Dynamic, ColliderMode::All, true};
        AddComponent(std::move(collider));
    }
    

    if (!GetComponent<Movement>())
    {
        std::unique_ptr<Movement> temp = std::make_unique<Movement>();
        movement = temp.get();
        movement->maxSpeed = 3.0f;
        AddComponent(std::move(temp));
    }
    else
    {
        movement = GetComponent<Movement>();
    }
    if (!GetComponent<PhysicsMaterial>())
    {
        std::unique_ptr<PhysicsMaterial> temp = std::make_unique<PhysicsMaterial>();
        temp->hasGravity = false;
        AddComponent(std::move(temp));
    }
    movement->usesPhysics = false;
    
    Entity::Initialize();
}

void Alien::Update(float _deltaTime)
{
    Entity::Update(_deltaTime);
    
    CircleCollider* collider = GetComponent<CircleCollider>();
    collider->CheckInBounds(World::GetGameManager()->GetXBounds(),
        World::GetGameManager()->GetYBounds(),
        World::GetGameManager()->GetZBounds());

    if (!playerTransform)
    {
        //playerTransform = World::GetInstance()->GetPlayer()->GetComponent<Transform>();
    }
    transform->LookAt(playerTransform->position);
}

void Alien::ClearComponents()
{
    movement = nullptr;
    Entity::ClearComponents();
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

void Alien::OnCollision(Entity* _otherEntity, vec3 _normal)
{
    if (dynamic_cast<Projectile*>(_otherEntity))
    {
        std::shared_ptr<Alien> self = std::static_pointer_cast<Alien>(shared_from_this());
        isActive = false;
        if (AliensLogic* logic = dynamic_cast<AliensLogic*>(World::GetGameManager()))
            logic->AlienDestroyed(self);
        return;
    }
    
    float velocityTowardPlane = dot(_normal, movement->speed);
    if (velocityTowardPlane > 0)
    {
        return;
    }
    if (dynamic_cast<Alien*>(_otherEntity))
    {
        movement->speed = _normal * length(movement->speed);
    }
}

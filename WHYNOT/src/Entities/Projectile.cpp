#include "Projectile.h"

#include "Components/CircleCollider.h"
#include "Graphics/Mesh.h"
#include "Components/Model.h"
#include "Components/Movement.h"
#include "Components/Transform.h"
#include "Graphics/Material.h"
#include "Managers/Renderer.h"
#include "Managers/World.h"
#include "Minigame1/AliensLogic.h"

unsigned int Projectile::counter = 0;


void Projectile::Initialize()
{
    isCamera = false;
    isLight = false;
    isRendered = true;
    hasCollision = true;

    if (!GetComponent<Transform>())
    {
        transformComp = std::make_shared<Transform>();
        AddComponent(transformComp);
    }

    if (!GetComponent<Model>())
    {
        Renderer::SetSphereVertex(0.2f, 32.f, 16.f);
        vector<float> vertex = Renderer::GetSphereVertex();
        vector<unsigned int> index = Renderer::GetShereIndex();

        std::shared_ptr<Material> mat = std::make_shared<Material>(vector<string>(), DEFAULT_VERTEX_SHADER_PATH,
            "shaders/neonFragment.glsl");
        mat->materialData.color = vec3(0.0f, 0.0f, 1.0f);
        mat->materialData.type = MaterialType::NEON;
        mat->materialData.shininess = 32;
    
        std::shared_ptr<Mesh> sphereMesh = std::make_shared<Mesh>(vertex, index, mat);
    
        std::shared_ptr<Model> model = std::make_shared<Model>();
        model->AddMesh(sphereMesh);
        AddComponent(model);
    }

    if (!GetComponent<CircleCollider>())
    {
        std::shared_ptr<CircleCollider> collider = std::make_shared<CircleCollider>(0.2);
        AddComponent(collider);
    }
    GetComponent<CircleCollider>()->OnOutOfBoundsDelegate.Bind(&Projectile::OnOutOfBounds, this);
    GetComponent<CircleCollider>()->CollisionDelegate.Bind(&Projectile::OnCollision, this);

    if (!GetComponent<Movement>())
    {
        movementComp = std::make_shared<Movement>();
        movementComp->maxSpeed = 40;
        AddComponent(movementComp);
    }
    else
    {
        movementComp = GetComponent<Movement>();
    }
    
    Entity::Initialize();
}

void Projectile::Update(float _deltaTime)
{
    GetComponent<Collider>()->CheckInBounds(vec2(-8, 8), vec2(-4, 12), vec2(-15, 16));
    Entity::Update(_deltaTime);
}

void Projectile::GetShot()
{
    std::shared_ptr<Player> player = World::GetInstance()->GetPlayer();
    if (!player)
    {
        Logger::Log(LogLevel::Error, "No Player Found");
        return;
    }
    std::shared_ptr<Transform> playerTransform = player->GetComponent<Transform>();
    
    isActive = true;
    movementComp->speed = playerTransform->forward * 5.f;
    transformComp->position = playerTransform->position + playerTransform->forward * 2.f;
}

void Projectile::DisableProjectile()
{
    isActive = false;
    movementComp->QuickStop();
}


void Projectile::OnCollision(const std::shared_ptr<Entity>& _otherEntity, vec3 normal)
{
    AliensLogic::GetInstance()->RemoveProjectile(std::static_pointer_cast<Projectile>(shared_from_this()));
}

void Projectile::OnOutOfBounds(vec3 _normal)
{
    AliensLogic::GetInstance()->RemoveProjectile(std::static_pointer_cast<Projectile>(shared_from_this()));
}
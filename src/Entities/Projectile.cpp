#include "Projectile.h"

#include "Components/CircleCollider.h"
#include "Graphics/Mesh.h"
#include "Components/Model.h"
#include "Components/Movement.h"
#include "Components/Transform.h"
#include "Graphics/Material.h"
#include "Managers/Renderer.h"
#include "Managers/World.h"
#include "GameManagers/AliensLogic.h"
#include "Utils/Parser.h"

unsigned int Projectile::counter = 0;


void Projectile::Initialize()
{
    isCamera = false;
    isLight = false;
    isRendered = true;
    hasCollision = true;

    if (!GetComponent<Transform>())
    {
        std::unique_ptr<Transform> temp = std::make_unique<Transform>();
        transformComp = temp.get();
        AddComponent(std::move(temp));
    }

    if (!GetComponent<Model>())
    {
        Renderer::SetSphereVertex(0.2f, 32.f, 16.f);
        vector<float> vertex = Renderer::GetSphereVertex();
        vector<unsigned int> index = Renderer::GetShereIndex();

        std::shared_ptr<Material> mat = std::make_shared<Material>(vector<string>(), DEFAULT_VERTEX_SHADER_PATH,
            "shaders/neonFragment.glsl");
        mat->materialData.color = vec4(0.0f, 0.0f, 1.0f, 1.0f);
        mat->materialData.type = MaterialType::NEON;
        mat->materialData.shininess = 32;
    
        std::unique_ptr<Mesh> sphereMesh = std::make_unique<Mesh>(vertex, vertex.size(), index, mat);
    
        std::unique_ptr model = std::make_unique<Model>();
        model->AddMesh(std::move(sphereMesh));
        AddComponent(std::move(model));
    }

    if (!GetComponent<CircleCollider>())
    {
        std::unique_ptr<CircleCollider> collider = std::make_unique<CircleCollider>(0.2);
        AddComponent(std::move(collider));
    }
    GetComponent<CircleCollider>()->profile = {ColliderType::Dynamic, ColliderMode::All};
    GetComponent<CircleCollider>()->OnOutOfBoundsDelegate.Bind(&Projectile::OnOutOfBounds, this);
    GetComponent<CircleCollider>()->CollisionDelegate.Bind(&Projectile::OnCollision, this);

    if (!GetComponent<Movement>())
    {
        std::unique_ptr<Movement> temp = std::make_unique<Movement>();
        movementComp = temp.get();
        movementComp->maxSpeed = 40;
        AddComponent(std::move(temp));
    }
    else
    {
        movementComp = GetComponent<Movement>();
    }
    alienLogicManager = static_cast<AliensLogic*>(World::GetGameManager());
    Entity::Initialize();
}

void Projectile::Update(float _deltaTime)
{
    GetComponent<Collider>()->CheckInBounds(Helper::GetXBounds(), Helper::GetYBounds(), Helper::GetZBounds());
    Entity::Update(_deltaTime);
}

void Projectile::ClearComponents()
{
    movementComp = nullptr;
    transformComp = nullptr;
    Entity::ClearComponents();
}

void Projectile::GetShot()
{
    std::shared_ptr<Player> player = World::GetInstance()->GetPlayer();
    if (!player)
    {
        Logger::Log(LogLevel::Error, "No Player Found");
        return;
    }
    if (!playerTransform)
    {
        playerTransform = player->GetComponent<Transform>();
    }
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
    if (alienLogicManager)
        alienLogicManager->RemoveProjectile(std::static_pointer_cast<Projectile>(shared_from_this()));
}

void Projectile::OnOutOfBounds(vec3 _normal)
{
    if (alienLogicManager)
        alienLogicManager->RemoveProjectile(std::static_pointer_cast<Projectile>(shared_from_this()));
}
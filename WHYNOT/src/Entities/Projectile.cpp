#include "Projectile.h"

#include "Components/CircleCollider.h"
#include "Graphics/Mesh.h"
#include "Components/Model.h"
#include "Components/Movement.h"
#include "Components/Transform.h"
#include "Graphics/Material.h"
#include "Managers/Renderer.h"
#include "Managers/World.h"

unsigned int Projectile::counter = 0;


void Projectile::Initialize()
{
    isCamera = false;
    isLight = false;
    isRendered = true;
    hasCollision = true;

    std::shared_ptr<Player> player = World::GetInstance()->GetPlayer();
    if (!player)
    {
        Logger::Log(LogLevel::Error, "No Player Found");
        return;
    }
    playerTransform = player->GetComponent<Transform>();

    std::shared_ptr<Transform> transform = std::make_shared<Transform>();
    transform->v_position = playerTransform->v_position + playerTransform->v_forward * 2.f;
    AddComponent(transform);
    
    Renderer::SetSphereVertex(0.5f, 32.f, 16.f);
    vector<float> vertex = Renderer::GetSphereVertex();
    vector<unsigned int> index = Renderer::GetShereIndex();

    std::shared_ptr<Material> mat = std::make_shared<Material>(vector<string>(), "shaders/vertex.glsl",
        "shaders/neonFragment.glsl");
    mat->materialData.color = vec3(0.0f, 0.0f, 1.0f);
    mat->materialData.type = MaterialType::NEON;
    mat->materialData.shininess = 32;
    
    std::shared_ptr<Mesh> sphereMesh = std::make_shared<Mesh>(vertex, index, mat);
    
    std::shared_ptr<Model> model = std::make_shared<Model>(sphereMesh);
    AddComponent(model);
    
    std::shared_ptr<CircleCollider> collider = std::make_shared<CircleCollider>(0.5);
    AddComponent(collider);
    
    std::shared_ptr<Movement> movement = std::make_shared<Movement>();
    movement->speed = playerTransform->v_forward * 5.f;
    AddComponent(movement);
    
    collider->OnOutOfBoundsDelegate.Bind(&Projectile::OnOutOfBounds, this);
    collider->CollisionDelegate.Bind(&Projectile::OnCollision, this);
    
    Entity::Initialize();
}

void Projectile::Update(float _deltaTime)
{
    GetComponent<Collider>()->CheckInBounds(vec2(-8, 8), vec2(-4, 12), vec2(-15, 16));
    
    Entity::Update(_deltaTime);
}


void Projectile::OnCollision(const std::shared_ptr<Entity>& _otherEntity, vec3 normal)
{
    Destroy();
}

void Projectile::OnOutOfBounds(vec3 _normal)
{
    Destroy();
}
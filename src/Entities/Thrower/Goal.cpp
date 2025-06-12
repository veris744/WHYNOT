#include "Goal.h"

#include <Components/Model.h>
#include <Components/Colliders/CircleCollider.h>
#include <GameManagers/ThrowerManager.h>
#include <Graphics/Material.h>
#include <Graphics/Mesh.h>
#include <Managers/Renderer.h>
#include <Managers/World.h>

unsigned int Goal::counter = 0;

void Goal::Initialize()
{
    if (!GetComponent<Transform>())
    {
        std::unique_ptr<Transform> temp = std::make_unique<Transform>();
        AddComponent(std::move(temp));
    }

    if (!GetComponent<Collider>())
    {
        std::unique_ptr<CircleCollider> collider = std::make_unique<CircleCollider>();
        collider->radius = 1;
        collider->profile = CollisionProfile(ColliderType::Static, ColliderMode::All, true);
        AddComponent(std::move(collider));
    }

    if (!GetComponent<Model>())
    {
        Renderer::SetSphereVertex(0.5f, 32.f, 16.f);
        vector<float> vertex = Renderer::GetSphereVertex();
        vector<unsigned int> index = Renderer::GetSphereIndex();

        std::shared_ptr<Material> mat = std::make_shared<Material>(vector<string>(), DEFAULT_VERTEX_SHADER_PATH,
            "shaders/debugFragment.glsl");
        mat->materialData.color = vec4(0.0f, 0.0f, 1.0f, 0.3f);
        mat->materialData.type = MaterialType::COLOR;

        std::unique_ptr<Mesh> sphereMesh = std::make_unique<Mesh>(vertex, vertex.size(), index, mat);

        std::unique_ptr model = std::make_unique<Model>();
        model->AddMesh(std::move(sphereMesh));
        model->hasTransparency = true;
        AddComponent(std::move(model));
    }

    isActive = true;
    material = GetComponent<Model>()->GetMeshes().front()->GetMaterial();
    Entity::Initialize();
}

void Goal::Update(float _deltaTime)
{
    Entity::Update(_deltaTime);

    // Detect new entries
    for (Entity* entity : entitiesEntering) {
        if (entitiesInside.count(entity) == 0) {
            int points;
            if (entity->GetProperty<int>("points", points))
                static_cast<ThrowerManager*>(World::GetGameManager())->AddPoints(points);
        }
    }

    // Detect exits
    for (Entity* entity : entitiesInside) {
        if (entitiesEntering.count(entity) == 0) {
            int points;
            if (entity->GetProperty<int>("points", points))
                static_cast<ThrowerManager*>(World::GetGameManager())->AddPoints(-points);
        }
    }
    
    entitiesInside = std::move(entitiesEntering);

    if (entitiesInside.size() > 0)
    {
        material->materialData.color = vec4(0.0f, 1.0f, 0.0f, 0.2f);
    }
    else
    {
        material->materialData.color = vec4(1.0f, 1.0f, 0.0f, 0.2f);
    }
}

void Goal::ClearComponents()
{
    Entity::ClearComponents();
}

void Goal::OnCollision(Entity* _otherEntity, vec3 normal)
{
    entitiesEntering.insert(_otherEntity);
}

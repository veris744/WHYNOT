#include "Goal.h"

#include <Components/Model.h>
#include <Components/Colliders/CircleCollider.h>
#include <Graphics/Material.h>
#include <Graphics/Mesh.h>
#include <Managers/Renderer.h>

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
        collider->radius = 4;
        collider->profile = CollisionProfile(ColliderType::Static, ColliderMode::All, true);
        AddComponent(std::move(collider));
    }

    if (!GetComponent<Model>())
    {
        Renderer::SetSphereVertex(0.2f, 32.f, 16.f);
        vector<float> vertex = Renderer::GetSphereVertex();
        vector<unsigned int> index = Renderer::GetSphereIndex();

        std::shared_ptr<Material> mat = std::make_shared<Material>(vector<string>(), DEFAULT_VERTEX_SHADER_PATH,
            "shaders/debugFragment.glsl");
        mat->materialData.color = vec4(0.0f, 0.0f, 1.0f, 0.3f);
        mat->materialData.type = MaterialType::COLOR;

        std::unique_ptr<Mesh> sphereMesh = std::make_unique<Mesh>(vertex, vertex.size(), index, mat);

        std::unique_ptr model = std::make_unique<Model>();
        model->AddMesh(std::move(sphereMesh));
        AddComponent(std::move(model));
    }

    isActive = true;
    Entity::Initialize();
}

void Goal::Update(float _deltaTime)
{
    Entity::Update(_deltaTime);

    // Detect new entries
    for (Entity* entity : entitiesEntering) {
        if (entitiesInside.count(entity) == 0) {
            //onEntityEnter(entity);
            Logger::Log(LogLevel::Info, entity->name + " has entered " + name);
        }
    }

    // Detect exits
    for (Entity* entity : entitiesInside) {
        if (entitiesEntering.count(entity) == 0) {
            Logger::Log(LogLevel::Info, entity->name + " has exited " + name);
        }
    }
    
    entitiesInside = std::move(entitiesEntering);
}

void Goal::ClearComponents()
{
    Entity::ClearComponents();
}

void Goal::OnOutOfBounds(vec3 _normal)
{

}

void Goal::OnCollision(Entity* _otherEntity, vec3 normal)
{
    entitiesEntering.insert(_otherEntity);
}

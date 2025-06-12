#include "ComponentFactory.h"

#include <Components/Colliders/BoxCollider.h>
#include <Components/Colliders/CapsuleCollider.h>
#include <Components/Colliders/PlaneCollider.h>
#include <Components/Colliders/SlopeCollider.h>
#include "Components/Transform.h"
#include "Components/Camera.h"
#include "Components/Colliders/CircleCollider.h"
#include "Components/LightSource.h"
#include "Graphics/Mesh.h"
#include "Components/Model.h"
#include "Components/Movement.h"
#include "Components/PhysicsMaterial.h"

#include "Entities/Entity.h"
#include "Graphics/Material.h"
#include "Graphics/Texture.h"
#include "Managers/Renderer.h"
#include "Reader.h"

using namespace Reader;
using namespace Reflection;

using ComponentCreator = std::function<void (const std::shared_ptr<Entity>& entity, const YAML::Node&)>;

std::shared_ptr<ComponentFactory> ComponentFactory::instance = nullptr;

void ComponentFactory::ComponentFactorySetup()
{
    RegisterComponent("TRANSFORM", [](const std::shared_ptr<Entity>& entity, const YAML::Node& data) ->
        void {
            std::unique_ptr<Transform> transform = std::make_unique<Transform>();
            deserialize(data, transform);
            entity->AddComponent(std::move(transform));
        });
    
    RegisterComponent("CAMERA", [](const std::shared_ptr<Entity>& entity, const YAML::Node& data) ->
        void {
            std::unique_ptr<Camera> camera = std::make_unique<Camera>();
            deserialize(data, camera);
            entity->AddComponent(std::move(camera));
        });
    
    RegisterComponent("MODEL", [](const std::shared_ptr<Entity>& entity, const YAML::Node& data) ->
        void {
            std::unique_ptr<Model> model = std::make_unique<Model>();
            deserialize(data, model);
            if (!model->path.empty())
            {
                model->LoadModel(model->path);
            }
            else if (data["mesh"])
            {
                model->AddMesh(ReadMesh(data["mesh"]));
            }
            entity->AddComponent(std::move(model));
        });
    
    RegisterComponent("LIGHT", [](const std::shared_ptr<Entity>& entity, const YAML::Node& data) ->
        void {
            std::unique_ptr<LightSource> ls = std::make_unique<LightSource>();
            deserialize(data, ls);
            entity->AddComponent(std::move(ls));
    });
    
    RegisterComponent("MOVEMENT", [](const std::shared_ptr<Entity>& entity, const YAML::Node& data) ->
        void {
            std::unique_ptr<Movement> move = std::make_unique<Movement>();
            deserialize(data, move);
            entity->AddComponent(std::move(move));
        });


    RegisterComponent("PHYSICS_MATERIAL", [](const std::shared_ptr<Entity>& entity, const YAML::Node& data) ->
        void {
            std::unique_ptr<PhysicsMaterial> physics_material = std::make_unique<PhysicsMaterial>();
            deserialize(data, physics_material);
            entity->AddComponent(std::move(physics_material));
        });

    
    RegisterComponent("COLLIDER", [](const std::shared_ptr<Entity>& entity, const YAML::Node& data) ->
        void {
            if (ReadString(data, "type") == "CIRCLE")
            {
                std::unique_ptr<CircleCollider> collider = std::make_unique<CircleCollider>();
                deserialize(data, collider);
                entity->AddComponent(std::move(collider));
            }
            else if (ReadString(data, "type") == "BOX")
            {
                std::unique_ptr<BoxCollider> collider = std::make_unique<BoxCollider>();
                deserialize(data, collider);
                entity->AddComponent(std::move(collider));
            }
            else if (ReadString(data, "type") == "CAPSULE")
            {
                std::unique_ptr<CapsuleCollider> collider = std::make_unique<CapsuleCollider>();
                deserialize(data, collider);
                entity->AddComponent(std::move(collider));
            }
            else if (ReadString(data, "type") == "PLANE")
            {
                std::unique_ptr<PlaneCollider> collider = std::make_unique<PlaneCollider>();
                deserialize(data, collider);
                entity->AddComponent(std::move(collider));
            }
            else if (ReadString(data, "type") == "SLOPE")
            {
                std::unique_ptr<SlopeCollider> collider = std::make_unique<SlopeCollider>();
                deserialize(data, collider);
                entity->AddComponent(std::move(collider));
            }
        });

}

void ComponentFactory::RegisterComponent(const std::string& type, ComponentCreator creator)
{
    creators[type] = std::move(creator);
}

void ComponentFactory::CreateComponent(const std::string& type, const std::shared_ptr<Entity>& entity, const YAML::Node& data) const
{
    auto it = creators.find(type);
    if (it != creators.end()) {
        return it->second(entity, data);
    }
    Logger::Log(LogLevel::FatalError, "Unknown entity type: " + type);
}


std::unique_ptr<Mesh> ComponentFactory::ReadMesh(const YAML::Node& asset)
{
    // INITIALIZE MESH VERTEX
    vector<float> vertex;
    vector<unsigned int> index = {};
    unsigned int elementCount = 3;
    unsigned int vertexCount = 0;
    
    switch (EnumRegistry::instance().fromString<PrimitiveType>(ReadString(asset, "primitive")))
    {
        case PrimitiveType::NONE:
            Logger::Log(LogLevel::FatalError, "Primitive type is NONE");
            return nullptr;
        case PrimitiveType::SPHERE:
            Helper::generateSphere(vertex, index,
            ReadFloat(asset, "radius", 0.5f),
            ReadInt(asset, "sectors", 32),
            ReadInt(asset, "stack", 16));
        break;
        case PrimitiveType::BOX:
            vertex = Renderer::GetCubeVertex();
        break;
        case PrimitiveType::PLANE:
            vertex = Renderer::GetPlaneVertex();
        break;
        case PrimitiveType::SLOPE:
            vertex = Renderer::GetSlopeVertex();
        break;
    }
    if (elementCount == 3)
    {
        vertexCount = vertex.size() / 8;
    }
    else if (elementCount == 2)
    {
        vertexCount = vertex.size() / 5;
    }
    else if (elementCount == 1)
    {
        vertexCount = vertex.size() / 3;
    }

    // INITIALIZE MATERIAL
    std::shared_ptr<Material> material = std::make_shared<Material>();
    deserialize(asset["material"], material);
    material->InitializeShader();

    if (material->materialData.type == MaterialType::TEXTURE)
    {
        if (material->wrapMode == WrapMode::REPEAT)
        {
            for (int i = 0; i < vertex.size(); i+=8)
            {
                vertex[i + 6] == 0.f ? vertex[i + 6] -= 0.5f : vertex[i + 6] += 0.5f;
                vertex[i + 7] == 0.f ? vertex[i + 7] -= 0.5f : vertex[i + 7] += 0.5f;
            }
        }
    }

    for (const string& texturePath : material->texturePaths)
    {
        std::shared_ptr<Texture> texturePtr = Renderer::instance().GetLoadedTexture(texturePath);
        if (texturePtr.use_count() == 0)
        {
            texturePtr = std::make_unique<Texture>(texturePath);
            Renderer::instance().textures_loaded.push_back(texturePtr);
        }
        material->AddTexture(texturePtr);
    }

    // INITIALIZE MESH
    std::unique_ptr<Mesh> mesh = nullptr;
    if (index.empty())
    {
        mesh = std::make_unique<Mesh>(vertex, vertexCount, material, elementCount);
    }
    else
    {
        mesh = std::make_unique<Mesh>(vertex, vertexCount, index, material, elementCount);
    }
    return mesh;
}

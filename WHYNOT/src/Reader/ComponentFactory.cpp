#include "ComponentFactory.h"

#include "Reader.h"
#include "Components/Transform.h"
#include "Components/Camera.h"
#include "Components/CircleCollider.h"
#include "Components/Collider.h"
#include "Components/LightSource.h"
#include "Graphics/Mesh.h"
#include "Components/Model.h"
#include "Components/Movement.h"
#include "Graphics/Material.h"
#include "Managers/Renderer.h"

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

    
    RegisterComponent("COLLIDER", [](const std::shared_ptr<Entity>& entity, const YAML::Node& data) ->
        void {
            if (ReadString(data, "type") == "CIRCLE")
            {
                std::unique_ptr<CircleCollider> collider = std::make_unique<CircleCollider>();
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
    
    switch (EnumRegistry::instance().fromString<PrimitiveType>(ReadString(asset, "primitive")))
    {
    case PrimitiveType::NONE:
        Logger::Log(LogLevel::FatalError, "Primitive type is NONE");
        return nullptr;
    case PrimitiveType::SPHERE:
        Helper::generateSphere(vertex, index,
            ReadFloat(asset, "radius"),
            ReadInt(asset, "sectors", 32),
            ReadInt(asset, "stack", 16));
        break;
    case PrimitiveType::BOX:
        vertex = Renderer::GetCubeVertex();
        break;
    }

    // INITIALIZE MATERIAL
    std::shared_ptr<Material> material = std::make_shared<Material>();
    deserialize(asset["material"], material);
    material->InitializeShader();

    for (const string& texturePath : material->texturePaths)
    {
        std::shared_ptr<Texture> texture = Renderer::GetInstance()->GetLoadedTexture(texturePath);
        if (!texture)
        {
            texture = std::make_unique<Texture>(texturePath);
            Renderer::GetInstance()->textures_loaded.push_back(texture);
        }
        material->AddTexture(texture);
    }

    // INITIALIZE MESH
    std::unique_ptr<Mesh> mesh = nullptr;
    if (index.empty())
    {
        mesh = std::make_unique<Mesh>(vertex, material);
    }
    else
    {
        mesh = std::make_unique<Mesh>(vertex, index, material);
    }
    return mesh;
}

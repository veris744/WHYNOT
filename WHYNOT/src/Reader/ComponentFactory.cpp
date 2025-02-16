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
            std::shared_ptr<Transform> transform = std::make_shared<Transform>();
            deserialize(data, transform);
            entity->AddComponent<Transform>(transform);
        });
    
    RegisterComponent("CAMERA", [](const std::shared_ptr<Entity>& entity, const YAML::Node& data) ->
        void {
            std::shared_ptr<Camera> camera = std::make_shared<Camera>();
            deserialize(data, camera);
            entity->AddComponent<Camera>(camera);
        });
    
    RegisterComponent("MESH", [](const std::shared_ptr<Entity>& entity, const YAML::Node& data) ->
        void {
            std::shared_ptr<Model> model = std::make_shared<Model>();
            model->AddMesh(ReadMesh(data));
            model->position = ReadVec3(data, "position");
            entity->AddComponent<Model>(model);
        
        });
    
    RegisterComponent("MODEL", [](const std::shared_ptr<Entity>& entity, const YAML::Node& data) ->
        void {
            std::shared_ptr<Model> model = std::make_shared<Model>();
            deserialize(data, model);
            model->LoadModel(model->path);
            entity->AddComponent<Model>(model);
        });
    
    RegisterComponent("LIGHT", [](const std::shared_ptr<Entity>& entity, const YAML::Node& data) ->
        void {
            std::shared_ptr<LightSource> ls = std::make_shared<LightSource>();
            deserialize(data, ls);
            entity->AddComponent<LightSource>(ls);
    });
    
    RegisterComponent("MOVEMENT", [](const std::shared_ptr<Entity>& entity, const YAML::Node& data) ->
        void {
            std::shared_ptr<Movement> move = std::make_shared<Movement>();
            deserialize(data, move);
            entity->AddComponent<Movement>(move);
        });

    
    RegisterComponent("COLLIDER", [](const std::shared_ptr<Entity>& entity, const YAML::Node& data) ->
        void {
            entity->hasCollision = true;
            if (ReadString(data, "type") == "CIRCLE")
            {
                std::shared_ptr<CircleCollider> collider = std::make_shared<CircleCollider>();
                deserialize(data, collider);
                entity->AddComponent<CircleCollider>(collider);
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


std::shared_ptr<Mesh> ComponentFactory::ReadMesh(const YAML::Node& asset)
{
    vector<float> vertex;
    vector<unsigned int> index = {};
    if (ReadString(asset, "primitive") == "CUBE")
    {
        vertex = Renderer::GetCubeVertex();
    }
    else if (ReadString(asset, "primitive") == "SPHERE")
    {
        Helper::generateSphere(vertex, index,
            ReadFloat(asset, "radius"),
            ReadInt(asset, "sectors", 32),
            ReadInt(asset, "stack", 16));
    }

    std::shared_ptr<Mesh> mesh = nullptr;
    std::shared_ptr<Material> material = std::make_shared<Material>();
    deserialize(asset["material"], material);
    material->InitializeShader();

    const YAML::Node& texturesYaml = asset["material"]["textures"];
    for (const YAML::Node& texturePath : texturesYaml)
    {
        string path = texturePath.as<string>();
        std::shared_ptr<Texture> texture = std::make_shared<Texture>(path);
        material->AddTexture(texture);
    }
    if (index.empty())
    {
        mesh = std::make_shared<Mesh>(vertex, material);
    }
    else
    {
        mesh = std::make_shared<Mesh>(vertex, index, material);
    }
    return mesh;
}

#include "ComponentFactory.h"

#include "Reader.h"
#include "Components/Transform.h"
#include "Components/Camera.h"
#include "Components/CircleCollider.h"
#include "Components/Collider.h"
#include "Components/LightSource.h"
#include "Graphics/Mesh.h"
#include "Components/Model.h"
#include "Components/PlayerController.h"
#include "Graphics/Material.h"
#include "Managers/Renderer.h"

using namespace Reader;

using ComponentCreator = std::function<void (const std::shared_ptr<Entity>& entity, const YAML::Node&)>;

std::shared_ptr<ComponentFactory> ComponentFactory::instance = nullptr;

void ComponentFactory::ComponentFactorySetup()
{
    RegisterComponent("TRANSFORM", [](const std::shared_ptr<Entity>& entity, const YAML::Node& data) ->
        void {
            entity->AddComponent<Transform>(ReadTransform(data));
        });
    
    RegisterComponent("CAMERA", [](const std::shared_ptr<Entity>& entity, const YAML::Node& data) ->
        void {
            entity->AddComponent<Camera>(ReadCamera(data));
        });
    
    RegisterComponent("MESH", [](const std::shared_ptr<Entity>& entity, const YAML::Node& data) ->
        void {
            std::shared_ptr<Model> model = std::make_shared<Model>(ReadMesh(data));
            model->position = ReadVec3(data, "position");
            entity->AddComponent<Model>(model);
        
        });
    
    RegisterComponent("MODEL", [](const std::shared_ptr<Entity>& entity, const YAML::Node& data) ->
        void {
            entity->AddComponent<Model>(ReadModel(data));
        });
    
    RegisterComponent("LIGHT", [](const std::shared_ptr<Entity>& entity, const YAML::Node& data) ->
        void {
            entity->AddComponent<LightSource>(ReadLight(data));
    });
    
    RegisterComponent("PLAYERCONTROLLER", [](const std::shared_ptr<Entity>& entity, const YAML::Node& data) ->
        void {
            entity->AddComponent<PlayerController>(ReadMoveComp(data));
        });

    
    RegisterComponent("COLLIDER", [](const std::shared_ptr<Entity>& entity, const YAML::Node& data) ->
        void {
            entity->hasCollision = true;
            if (ReadString(data, "type") == "CIRCLE")
            {
                entity->AddComponent<CircleCollider>(ReadCircleCollider(data));
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



std::shared_ptr<Transform> ComponentFactory::ReadTransform(const YAML::Node& asset)
{
    std::shared_ptr<Transform> transform = std::make_shared<Transform>();
    
    transform->v_position = ReadVec3(asset, "position");
    transform->v_scale = ReadVec3(asset, "scale", vec3(1.0f, 1.0f, 1.0f));
    transform->v_rotation.SetRotation(ReadVec3(asset, "rotation"));
    
    return transform;
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
    if (index.empty())
    {
        mesh = std::make_shared<Mesh>(vertex, ReadMaterial(asset));
    }
    else
    {
        mesh = std::make_shared<Mesh>(vertex, index, ReadMaterial(asset));
    }
    return mesh;
}

std::shared_ptr<Model> ComponentFactory::ReadModel(const YAML::Node& asset)
{
    std::shared_ptr<Model> model = std::make_shared<Model>(ReadString(asset,"path").c_str());
    model->position = ReadVec3(asset, "position");
    model->invertTexture = ReadBool(asset, "invert");
    return model;
}

std::shared_ptr<Camera> ComponentFactory::ReadCamera(const YAML::Node& asset)
{
    std::shared_ptr<Camera> camera = std::make_shared<Camera>();
    return camera;
}

std::shared_ptr<LightSource> ComponentFactory::ReadLight(const YAML::Node& asset)
{
    std::shared_ptr<LightSource> light = std::make_shared<LightSource>();
    LightData lightData = LightData();
    const YAML::Node& data = asset["lightData"];
    lightData.position = vec4(ReadVec3(data, "position"), 1.f);  
    lightData.color = vec4(ReadVec3(data, "color", vec3(1.f)), 1.f); 
    lightData.direction = vec4(ReadVec3(data, "direction", vec3(1.f)), 1.f); 
    lightData.specular = ReadFloat(data, "specular"); 
    lightData.diffuse = ReadFloat(data, "diffuse"); 
    lightData.ambient = ReadFloat(data, "ambient"); 
    lightData.constantAtt = ReadFloat(data, "constantAtt"); 
    lightData.linearAtt = ReadFloat(data, "linearAtt"); 
    lightData.specular = ReadFloat(data, "quadAtt"); 
    lightData.angle = ReadFloat(data, "angle"); 
    lightData.outerAngle = ReadFloat(data, "outerAngle");

    if (ReadString(data, "type") == "AMBIENT")
        lightData.type = LightSourceType::AMBIENT;
    else if (ReadString(data, "type") == "DIRECTIONAL")
        lightData.type = LightSourceType::DIRECTIONAL; 
    else if (ReadString(data, "type") == "SPOT")
        lightData.type = LightSourceType::SPOT; 
    else if (ReadString(data, "type") == "POINT")
        lightData.type = LightSourceType::POINT; 
    
    light->lightData = lightData;
    return light;
}

std::shared_ptr<Material> ComponentFactory::ReadMaterial(const YAML::Node& asset)
{
    MaterialData materialData;
    materialData.ambient = ReadInt(asset, "ambient"); 
    materialData.diffuse = ReadInt(asset, "diffuse"); 
    materialData.specular = ReadInt(asset, "specular"); 
    materialData.shininess = ReadFloat(asset, "diffuse"); 
    materialData.color = ReadVec3(asset, "color");
    
    std::string type = ReadString(asset, "type");
    if (type == "TEXTURE") {
        materialData.type = MaterialType::TEXTURE;
    } 
    else if (type == "COLOR") {
        materialData.type = MaterialType::COLOR;
    } 
    else if (type == "NEON") {
        materialData.type = MaterialType::NEON;
    } 
        
    vector<string> textures;
    const YAML::Node& texturesYaml = asset["textures"];
    for (const YAML::Node& texture : texturesYaml)
    {
        string temp = texture.as<string>();
        textures.push_back(temp);
    }
    string vertexShader= ReadString(asset, "vertexShader");
    string fragmentShader= ReadString(asset, "fragmentShader");
    
    std::shared_ptr<Material> material = std::make_shared<Material>(
        textures, vertexShader, fragmentShader, materialData);
    
    return material;
}

std::shared_ptr<PlayerController> ComponentFactory::ReadMoveComp(const YAML::Node& asset)
{
    std::shared_ptr<PlayerController> moveComp = std::make_shared<PlayerController>();
    moveComp->maxSpeed = ReadFloat(asset, "maxSpeed"); 
    moveComp->maxAcceleration = ReadFloat(asset, "maxAcceleration"); 
    return moveComp;
}

std::shared_ptr<CircleCollider> ComponentFactory::ReadCircleCollider(const YAML::Node& asset)
{
    std::shared_ptr<CircleCollider> circleCollider = std::make_shared<CircleCollider>(ReadFloat(asset, "radius"));
    return circleCollider;
}

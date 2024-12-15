#include "ComponentFactory.h"

#include "Components/Transform.h"
#include "Components/Camera.h"
#include "Components/LightSource.h"
#include "Components/Mesh.h"
#include "Components/Model.h"
#include "Graphics/Material.h"
#include "Managers/Renderer.h"

using ComponentCreator = std::function<void (const std::shared_ptr<Entity>& entity, const YAML::Node&)>;

ComponentFactory* ComponentFactory::instance = nullptr;

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
            entity->AddComponent<Model>(std::make_shared<Model>(ReadMesh(data)));
        });
    
    RegisterComponent("MODEL", [](const std::shared_ptr<Entity>& entity, const YAML::Node& data) ->
        void {
            entity->AddComponent<Model>(ReadModel(data));
        });
    
    RegisterComponent("LIGHT", [](const std::shared_ptr<Entity>& entity, const YAML::Node& data) ->
        void {
            entity->AddComponent<LightSource>(ReadLight(data));
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
    throw std::runtime_error("Unknown entity type: " + type);
}



std::shared_ptr<Transform> ComponentFactory::ReadTransform(const YAML::Node& asset)
{
    std::shared_ptr<Transform> transform = std::make_shared<Transform>();
    
    transform->v_position = vec3(asset["position"][0].as<float>(),
         asset["position"][1].as<float>(), asset["position"][2].as<float>());    
    transform->v_scale = vec3(asset["scale"][0].as<float>(),
        asset["scale"][1].as<float>(), asset["scale"][2].as<float>());
    transform->v_rotation.SetRotation(asset["rotation"][0].as<float>(),
        asset["rotation"][1].as<float>(), asset["rotation"][2].as<float>());
    
    return transform;
}

std::shared_ptr<Mesh> ComponentFactory::ReadMesh(const YAML::Node& asset)
{
    vector<float> vertex;
    unsigned int vertexCount = 0;
    if (asset["primitive"].as<string>() == "CUBE")
    {
        vertex = Renderer::cubeVertex;
        vertexCount = 36;
    }

    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(vertex, vertexCount, ReadMaterial(asset));
    return mesh;
}

std::shared_ptr<Model> ComponentFactory::ReadModel(const YAML::Node& asset)
{
    std::shared_ptr<Model> model = std::make_shared<Model>(asset["path"].as<string>().c_str());
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
        if (data["position"])
        {
            lightData.position = vec4(data["position"][0].as<float>(),
            data["position"][1].as<float>(), data["position"][2].as<float>(), 1.f);  
        }
        if (data["color"])
        {
            lightData.color = vec4(data["color"][0].as<float>(),
             data["color"][1].as<float>(), data["color"][2].as<float>(), 1.f); 
        }
        if (data["direction"])
        {
            lightData.direction = vec4(data["direction"][0].as<float>(),
             data["direction"][1].as<float>(), data["direction"][2].as<float>(), 1.f); 
        }
        if (data["specular"])
        {
            lightData.specular = data["specular"].as<float>(); 
        }
        if (data["diffuse"])
        {
            lightData.diffuse = data["diffuse"].as<float>(); 
        }
        if (data["ambient"])
        {
            lightData.ambient = data["ambient"].as<float>(); 
        }
        if (data["constantAtt"])
        {
            lightData.constantAtt = data["constantAtt"].as<float>(); 
        }
        if (data["linearAtt"])
        {
            lightData.linearAtt = data["linearAtt"].as<float>(); 
        }
        if (data["quadAtt"])
        {
            lightData.specular = data["quadAtt"].as<float>(); 
        }
        if (data["quadAtt"])
        {
            lightData.specular = data["quadAtt"].as<float>(); 
        }
        if (data["angle"])
        {
            lightData.angle = data["angle"].as<float>(); 
        }
        if (data["outerAngle"])
        {
            lightData.outerAngle = data["outerAngle"].as<float>(); 
        }
        if (data["type"])
        {
            if (data["type"].as<string>() == "AMBIENT")
                lightData.type = LightSourceType::AMBIENT;
            else if (data["type"].as<string>() == "DIRECTIONAL")
                lightData.type = LightSourceType::DIRECTIONAL; 
            else if (data["type"].as<string>() == "SPOT")
                lightData.type = LightSourceType::SPOT; 
            else if (data["type"].as<string>() == "POINT")
                lightData.type = LightSourceType::POINT; 
        }

        light->lightData = lightData;
        return light;
}

std::shared_ptr<Material> ComponentFactory::ReadMaterial(const YAML::Node& asset)
{
    MaterialData materialData;
    if (asset["ambient"])
    {
        materialData.ambient = asset["ambient"].as<int>(); 
    }
    if (asset["diffuse"])
    {
        materialData.diffuse = asset["diffuse"].as<int>(); 
    }
    if (asset["specular"])
    {
        materialData.specular = asset["specular"].as<int>(); 
    }
    if (asset["shininess"])
    {
        materialData.shininess = asset["shininess"].as<float>(); 
    }
        
    vector<string> textures;
    const YAML::Node& texturesYaml = asset["textures"];
    for (const YAML::Node& texture : texturesYaml)
    {
        string temp = texture.as<string>();
        textures.push_back(temp);
    }
    string vertexShader;
    if (asset["vertexShader"])
    {
        vertexShader = asset["vertexShader"].as<string>();
    }
    string fragmentShader;
    if (asset["fragmentShader"])
    {
        fragmentShader = asset["fragmentShader"].as<string>();
    }
    std::shared_ptr<Material> material = std::make_shared<Material>(
        textures, vertexShader, fragmentShader, materialData);
    
    return material;
}

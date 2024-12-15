#include "AssetReader.h"

#include "Components/Camera.h"
#include "Components/LightSource.h"
#include "Components/Mesh.h"
#include "Components/Model.h"
#include "Components/Transform.h"
#include "Graphics/Material.h"
#include "Managers/Renderer.h"
#include "Managers/World.h"



void AssetReader::ReadAssets(const char* filePath)
{
    YAML::Node fileYaml = YAML::LoadFile(filePath);
    if (fileYaml["assets"])
    {
        for (const YAML::Node& asset : fileYaml["assets"])
        {
            SaveEntity(asset);
        }
    }
}


void AssetReader::SaveEntity(const YAML::Node& asset)
{
    std::shared_ptr<Entity> entity = std::make_shared<Entity>(asset["name"].as<std::string>(),
        asset["isCamera"].as<bool>(), asset["isLight"].as<bool>());
    entity->isRendered = asset["isRendered"].as<bool>();

    for (const YAML::Node& component : asset["components"])
    {
        GetComponent(entity, component);
    }

    World::GetInstance()->AddEntity(entity);
}

void AssetReader::GetComponent(const std::shared_ptr<Entity>& entity, const YAML::Node& asset)
{
    string name = asset["name"].as<std::string>();
    
    if (name == "TRANSFORM")
    {
        entity->AddComponent<Transform>(ReadTransform(asset));
    }
    if (name == "MESH")
    {
        std::shared_ptr<Model> model = std::make_shared<Model>(ReadMesh(asset));
        entity->AddComponent<Model>(model);
    }
    if (name == "MODEL")
    {
        entity->AddComponent<Model>(ReadModel(asset));
    }
    if (name == "CAMERA")
    {
        entity->AddComponent<Camera>(ReadCamera(asset));
    }
    if (name == "LIGHT")
    {
        entity->AddComponent<LightSource>(ReadLight(asset));
    }
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


std::shared_ptr<Transform> AssetReader::ReadTransform(const YAML::Node& asset)
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

std::shared_ptr<Mesh> AssetReader::ReadMesh(const YAML::Node& asset)
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

std::shared_ptr<Model> AssetReader::ReadModel(const YAML::Node& asset)
{
    std::shared_ptr<Model> model = std::make_shared<Model>(asset["path"].as<string>().c_str());
    return model;
}

std::shared_ptr<Camera> AssetReader::ReadCamera(const YAML::Node& asset)
{
    std::shared_ptr<Camera> camera = std::make_shared<Camera>();
    return camera;
}

std::shared_ptr<LightSource> AssetReader::ReadLight(const YAML::Node& asset)
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

std::shared_ptr<Material> AssetReader::ReadMaterial(const YAML::Node& asset)
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
    string vertexShader = "";
    if (asset["vertexShader"])
    {
        vertexShader = asset["vertexShader"].as<string>();
    }
    string fragmentShader = "";
    if (asset["fragmentShader"])
    {
        fragmentShader = asset["fragmentShader"].as<string>();
    }
    std::shared_ptr<Material> material = std::make_shared<Material>(
        textures, vertexShader, fragmentShader, materialData);
    
    return material;
}
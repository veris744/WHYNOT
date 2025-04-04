#include "Material.h"

#include "Shader.h"
#include "Texture.h"
#include "Components/LightSource.h"
#include "Managers/Renderer.h"
#include "Managers/World.h"
#include "Utils/Parser.h"


struct gpuLightData
{
    vec4  position = vec4(0.f, 0.f,0.f,1);
    vec4  color = vec4(0, 0,0,1);
    vec3  direction = vec3(0);

    float specular    = 0.f;
    float diffuse     = 0.f;
    float ambient     = 0.f;

    float constantAtt = 0.f;
    float linearAtt = 0.f;
    float quadAtt = 0.f;

    float angle = 0.f;
    float outerAngle = 0.f;

    LightSourceType type = LightSourceType::DIRECTIONAL;

    gpuLightData(const LightData& other)
        : position(other.position), 
          color(other.color),
          direction(other.direction),
          specular(other.specular),
          diffuse(other.diffuse),
          ambient(other.ambient),
          constantAtt(other.constantAtt),
          linearAtt(other.linearAtt),
          quadAtt(other.quadAtt),
          angle(other.angle),
          outerAngle(other.outerAngle),
          type(other.type) {} 
};

Material::Material(const char* _texturePath, const string& _vertexShaderPath, const string& _fragmentShaderPath, MaterialData _materialData)
        : materialData(_materialData), vertexShaderPath(_vertexShaderPath), fragmentShaderPath(_fragmentShaderPath)
{
    InitializeShader();
    if (std::strcmp(_texturePath, ""))
    {
        std::shared_ptr<Texture> texture = Renderer::instance().GetLoadedTexture(_texturePath);
        if (!texture)
        {
            texture = std::make_shared<Texture>(_texturePath);
            Renderer::instance().textures_loaded.push_back(texture);
        }
        textures.push_back(texture);
    }
}

Material::Material(const vector<string>& _texturePaths, const string& _vertexShaderPath,
    const string& _fragmentShaderPath, MaterialData _materialData)
        : materialData(_materialData), vertexShaderPath(_vertexShaderPath), fragmentShaderPath(_fragmentShaderPath)
{
    InitializeShader();

    for (string texturePath : _texturePaths)
    {
        std::shared_ptr<Texture> texture = Renderer::instance().GetLoadedTexture(texturePath);
        if (!texture)
        {
            texture = std::make_shared<Texture>(texturePath);
            Renderer::instance().textures_loaded.push_back(texture);
        }
        textures.push_back(texture);
    }
    if (textures.size() == 2)
    {
        materialData.specular = 1;
    }
    
}

Material::Material(const vector<std::shared_ptr<Texture>>& _textures, const string& _vertexShaderPath,
    const string& _fragmentShaderPath, MaterialData _materialData)
        : textures(_textures)
        , materialData(_materialData)
        , vertexShaderPath(_vertexShaderPath)
        , fragmentShaderPath(_fragmentShaderPath)
{
    InitializeShader();
}

void Material::InitializeShader()
{
    bool skip = false;
    for (const auto& shaderLoaded : Renderer::instance().shaders_loaded)
    {
        if (shaderLoaded->vertexShaderPath == vertexShaderPath
            && shaderLoaded->fragmentShaderPath == fragmentShaderPath)
        {
            shader = shaderLoaded;
            skip = true;
            break;
        }
    }
    if (!skip)
    {
        shader = std::make_shared<Shader>(vertexShaderPath, fragmentShaderPath);
        shader->Compile();
    
        Renderer::instance().shaders_loaded.push_back(shader);
    }
}

void Material::AddTexture(const std::shared_ptr<Texture>& _texture)
{
    _texture->wrapMode = wrapMode;
    _texture->filterMode = filterMode;
    _texture->mipmapMode = mipmapMode;
    textures.push_back(_texture);
}

void Material::SetUniforms(const mat4& _model, const mat4& _view, const mat4& _projection, const vec3& _viewPosition)
{
    shader->SetUniformMat4("uModel", _model);
    shader->SetUniformMat4("uView", _view);
    shader->SetUniformMat4("uProjection", _projection);
    shader->SetUniformVec3("uViewPos", _viewPosition);
    
    shader->SetUniformInt("uAmbient", materialData.ambient);
    shader->SetUniformInt("uDiffuse", materialData.diffuse);
    shader->SetUniformInt("uSpecular", materialData.specular);
    shader->SetUniformFloat("uShininess", materialData.shininess);  
    if (materialData.type == MaterialType::TEXTURE)
    {
        shader->SetUniformInt("uSampler2D", 0);
    }
    else if (materialData.type == MaterialType::COLOR)
    {
        shader->SetUniformVec4("uColor", materialData.color);
    }
    else if (materialData.type == MaterialType::NEON)
    {
        shader->SetUniformVec3("uColor", materialData.color);
        shader->SetUniformFloat("uGlowIntensity", 5.f);
        shader->SetUniformFloat("uEdgeGlow", 1.f);
    }
    
    std::vector<gpuLightData> gpuLights;
    for (const auto& light : World::GetInstance()->GetLightDataList()) {
        gpuLights.push_back(gpuLightData(light));  // Extract only GPU-relevant data
    }
    
    shader->SetUniformObject("u_Lights", 1,
        World::GetInstance()->GetLightCount(),
        sizeof(gpuLightData), gpuLights.data());
    shader->SetUniformInt("uNumLights", World::GetInstance()->GetLightCount());


#ifdef _DEBUG
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL Error: " << error << std::endl;
        return;
    }
#endif
}


void Material::BindTexture() const
{
    for (int i = 0; i < textures.size(); i++)
    {
        textures[i]->Activate(i);
        textures[i]->Bind();
    }
}


void Material::UnbindTexture() const
{
    if (!textures.empty())   
        textures[0]->Unbind();
}

void Material::BindShader() const
{
    shader->Bind();
}

void Material::UnbindShader() const
{
    shader->Unbind();
}

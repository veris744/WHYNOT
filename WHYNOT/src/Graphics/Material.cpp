#include "Material.h"

#include "Shader.h"
#include "Texture.h"
#include "Components/LightSource.h"
#include "Managers/Renderer.h"
#include "Managers/World.h"


Material::Material(const char* _texturePath, const string& _vertexShaderPath, const string& _fragmentShaderPath, MaterialData _materialData)
        : materialData(_materialData)
{
    string vertexShader = _vertexShaderPath.empty() ? string(DEFAULT_VERTEX_SHADER_PATH) : _vertexShaderPath;
    string fragmentShader = _fragmentShaderPath.empty() ? string(DEFAULT_FRAGMENT_SHADER_PATH) : _fragmentShaderPath;
    
    std::shared_ptr<Texture> texture = std::make_shared<Texture>(_texturePath);

    textures.push_back(texture);
    
    bool skip = false;
    for (const auto& shaderLoaded : Renderer::GetInstance()->shaders_loaded)
    {
        if (shaderLoaded->vertexShaderSource.compare(vertexShader) == 0
            && shaderLoaded->fragmentShaderSource.compare(fragmentShader) == 0)
        {
            shader = shaderLoaded;
            skip = true;
            break;
        }
    }

    if (!skip)
    {
        shader = std::make_shared<Shader>(vertexShader, fragmentShader);
        shader->Compile();
    
        Renderer::GetInstance()->shaders_loaded.push_back(shader);
    }
    
    Renderer::GetInstance()->shaders_loaded.push_back(shader);
}

Material::Material(const vector<string>& _texturePaths, const string& _vertexShaderPath,
    const string& _fragmentShaderPath, MaterialData _materialData)
        : materialData(_materialData)
{
    string vertexShader = _vertexShaderPath.empty() ? string(DEFAULT_VERTEX_SHADER_PATH) : _vertexShaderPath;
    string fragmentShader = _fragmentShaderPath.empty() ? string(DEFAULT_FRAGMENT_SHADER_PATH) : _fragmentShaderPath;
    
    
    unsigned int i = 0;
    for (string texturePath : _texturePaths)
    {
        std::shared_ptr<Texture> texture = std::make_shared<Texture>(texturePath);

        textures.push_back(texture);
        i++;
    }    
    bool skip = false;
    for (const auto& shaderLoaded : Renderer::GetInstance()->shaders_loaded)
    {
        if (shaderLoaded->vertexShaderSource.compare(vertexShader) == 0
            && shaderLoaded->fragmentShaderSource.compare(fragmentShader) == 0)
        {
            shader = shaderLoaded;
            skip = true;
            break;
        }
    }

    if (!skip)
    {
        shader = std::make_shared<Shader>(vertexShader, fragmentShader);
        shader->Compile();
    
        Renderer::GetInstance()->shaders_loaded.push_back(shader);
    }
    
    Renderer::GetInstance()->shaders_loaded.push_back(shader);
}

Material::Material(const vector<std::shared_ptr<Texture>>& _textures, const string& _vertexShaderPath,
    const string& _fragmentShaderPath, MaterialData _materialData)
        : textures(_textures)
        , materialData(_materialData)
{
    string vertexShader = _vertexShaderPath.empty() ? string(DEFAULT_VERTEX_SHADER_PATH) : _vertexShaderPath;
    string fragmentShader = _fragmentShaderPath.empty() ? string(DEFAULT_FRAGMENT_SHADER_PATH) : _fragmentShaderPath;

    bool skip = false;
    for (const auto& shaderLoaded : Renderer::GetInstance()->shaders_loaded)
    {
        if (shaderLoaded->vertexShaderSource.compare(vertexShader) == 0
            && shaderLoaded->fragmentShaderSource.compare(fragmentShader) == 0)
        {
            shader = shaderLoaded;
            skip = true;
            break;
        }
    }

    if (!skip)
    {
        shader = std::make_shared<Shader>(vertexShader, fragmentShader);
        shader->Compile();
    
        Renderer::GetInstance()->shaders_loaded.push_back(shader);
    }
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
    shader->SetUniformObject("u_Lights", 1,
        World::GetInstance()->GetLightCount(),
        sizeof(LightSource), World::GetInstance()->GetLightDataList().data());

    shader->SetUniformInt("uNumLights", World::GetInstance()->GetLightCount());
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

#include "Renderer.h"

#include "Graphics/Shader.h"
#include "World.h"
#include "Graphics/Mesh.h"
#include "Components/Model.h"
#include "Graphics/Texture.h"
#include "Utils/Debugger.h"


class Model;

vector<float> Renderer::sphereVertex = {};
vector<unsigned int> Renderer::sphereIndex = {};
const vector<float> Renderer::cubeVertex = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
    
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
    
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
    
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

Renderer::Renderer()
{

}


void Renderer::Initialize()
{
    // glEnable(GL_DEPTH_TEST);
    // glEnable(GL_BLEND);
}

void Renderer::Render()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glDisable(GL_BLEND);
    
    
    for (const auto& entity : World::GetInstance()->GetEntities())
    {
        if (!entity.second->isActive)
        {
            continue;
        }
        if (entity.second->IsRendered())
        {
            Model* model = entity.second->GetComponent<Model>();
            if (!model)
            {
                Logger::Log<Renderer>(LogLevel::Warning,  "Renderable entity does not have a model");
            }
            if (!model->enableCulling)  glDisable(GL_CULL_FACE);
            model->Render();
            if (!model->enableCulling)  glEnable(GL_CULL_FACE);
        }

#ifdef _DEBUG
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        if (entity.second->debugEnabled)
        {
            for (const auto& comp : entity.second->GetComponents())
            {
                if (comp->debugEnabled)
                {
                    comp->RenderDebug();
                }
            }
        }
    }
    Debugger::Render();
    glDisable(GL_BLEND);
#endif
}
void Renderer::Clear()
{
    for (const auto& entity : World::GetInstance()->GetEntities())
    {
        if (!entity.second->IsRendered() || !entity.second->isActive)
        {
            continue;
        }
        Model* model = entity.second->GetComponent<Model>();
        if (!model || !model->HasMeshes())
        {
            Logger::Log<Renderer>(LogLevel::Warning,  "Renderable entity does not have a model");
            return;
        }
        model->Clear();
    }
    Debugger::Clear();
}

void Renderer::CleanUp()
{
    for (const auto& texture : textures_loaded)
    {
        texture->CleanUp();
    }
    for (const auto& shader : shaders_loaded)
    {
        shader->CleanUp();
    }
    textures_loaded.clear();
    shaders_loaded.clear();
}

const std::shared_ptr<Texture>& Renderer::GetLoadedTexture(const string& path) const
{
    for (const auto& texture : textures_loaded)
    {
        if (texture->GetPath() == path) return texture;
    }
    return nullptr;
}

const std::shared_ptr<Shader>& Renderer::GetLoadedShader(const string& pathVertex, const string& pathFragment) const
{
    for (const auto& shader : shaders_loaded)
    {
        if (shader->vertexShaderPath == pathVertex && shader->fragmentShaderPath == pathFragment) return shader;
    }
    return nullptr;
}

#include "Renderer.h"

#include <stack>

#include "Renderer2D.h"
#include "Graphics/Shader.h"
#include "World.h"
#include "Graphics/Mesh.h"
#include "Components/Model.h"
#include "Graphics/Texture.h"
#include "Utils/Debugger.h"


auto AddTriangle = [&](int a, int b, int c, std::vector<unsigned int>& indices) {
    indices.push_back(static_cast<unsigned int>(a));
    indices.push_back(static_cast<unsigned int>(b));
    indices.push_back(static_cast<unsigned int>(c));
};

std::vector<float> Renderer::sphereVertex = {};
std::vector<unsigned int> Renderer::sphereIndex = {};

const std::vector<float> Renderer::planeVertex = {
    -0.5f,  0.f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    };
const std::vector<float> Renderer::cubeVertex = {
    // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

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
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    };

const std::vector<float> Renderer::slopeVertex = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.707f,  0.707f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.707f,  0.707f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.707f,  0.707f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.707f,  0.707f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.707f,  0.707f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.707f,  0.707f,  0.0f, 0.0f
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
    glDepthMask(GL_TRUE);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    std::stack<Model*> transparentObjects;
    
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
            // Don't render yet with transparency
            if (model->hasTransparency)
            {
                transparentObjects.push(model);
                continue;
            }

            if (!model->enableCulling)  glDisable(GL_CULL_FACE);
            model->Render();
            if (!model->enableCulling)  glEnable(GL_CULL_FACE);
        }

#ifdef _DEBUG
        //Prepare debug rendering
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
#endif
    }


    // Render objects with transparency (and debug if on)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    int nTransparent = transparentObjects.size();
    for (int i = 0; i < nTransparent; ++i)
    {
        Model* model = transparentObjects.top();
        if (!model->enableCulling)  glDisable(GL_CULL_FACE);
        model->Render();
        if (!model->enableCulling)  glEnable(GL_CULL_FACE);
        transparentObjects.pop();
    }

#ifdef _DEBUG
    Debugger::Render();
#endif

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
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

const std::shared_ptr<Texture>& Renderer::GetLoadedTexture(const std::string& path) const
{
    for (const auto& texture : textures_loaded)
    {
        if (texture->GetPath() == path) return texture;
    }
    return nullptr;
}

const std::shared_ptr<Shader>& Renderer::GetLoadedShader(const std::string& pathVertex, const std::string& pathFragment) const
{
    for (const auto& shader : shaders_loaded)
    {
        if (shader->vertexShaderPath == pathVertex && shader->fragmentShaderPath == pathFragment) return shader;
    }
    return nullptr;
}

const void Renderer::GenerateCapsuleVertex(float radius, float height, std::vector<float>& vertices, std::vector<unsigned int>& indices)
{
    const int slices = 16;
    const int stacks = 8;
    float bodyHalfHeight = (height * 0.5f) - radius;

    // Helper: add vertex
    auto addVertex = [&](glm::vec3 pos, glm::vec3 normal, glm::vec2 uv) {
        vertices.insert(vertices.end(), {
            pos.x, pos.y, pos.z,
            normal.x, normal.y, normal.z,
            uv.x, uv.y
        });
    };

    // --- Body (cylinder) ---
    for (int i = 0; i <= slices; ++i)
    {
        float theta = i * 2.0f * M_PI / slices;
        float x = cos(theta);
        float z = sin(theta);
        glm::vec3 normal = normalize(glm::vec3(x, 0, z));
        glm::vec2 uv(i / float(slices), 0.5f);

        glm::vec3 bottom = glm::vec3(x * radius, -bodyHalfHeight, z * radius);
        glm::vec3 top = glm::vec3(x * radius, +bodyHalfHeight, z * radius);

        addVertex(bottom, normal, uv);
        addVertex(top, normal, uv);
    }

    // Triangles for cylinder sides
    int bodyStart = 0;
    for (int i = 0; i < slices; ++i)
    {
        int start = bodyStart + i * 2;
        // indices.insert(indices.end(), {
        //     start, start + 1, start + 2,
        //     start + 1, start + 3, start + 2
        // });
        AddTriangle(start, start + 1, start + 2, indices);
        AddTriangle(start + 1, start + 3, start + 2, indices);
    }

    // --- Hemispheres (top & bottom) ---
    int vertOffset = vertices.size() / 8;

    for (int hemisphere = -1; hemisphere <= 1; hemisphere += 2)
    {
        for (int i = 0; i <= stacks; ++i)
        {
            float phi = (i * M_PI * 0.5f) / stacks;
            for (int j = 0; j <= slices; ++j)
            {
                float theta = j * 2.0f * M_PI / slices;
                float x = sin(phi) * cos(theta);
                float y = cos(phi);
                float z = sin(phi) * sin(theta);
                glm::vec3 normal = normalize(glm::vec3(x, y * hemisphere, z));
                glm::vec3 pos = glm::vec3(x * radius, y * radius * hemisphere + hemisphere * bodyHalfHeight, z * radius);
                glm::vec2 uv(j / float(slices), i / float(stacks));
                addVertex(pos, normal, uv);
            }
        }

        int ringVerts = slices + 1;
        int start = vertOffset;
        for (int i = 0; i < stacks; ++i)
        {
            for (int j = 0; j < slices; ++j)
            {
                int i0 = start + i * ringVerts + j;
                int i1 = i0 + ringVerts;
                int i2 = i0 + 1;
                int i3 = i1 + 1;

                if (hemisphere == 1)
                {
                    // top cap
                    //indices.insert(indices.end(), { i0, i1, i2, i2, i1, i3 });
                    AddTriangle(i0, i2, i1, indices);
                    AddTriangle(i2, i3, i1, indices);
                }
                else
                {
                    // bottom cap
                    //indices.insert(indices.end(), { i0, i2, i1, i2, i3, i1 });
                    AddTriangle(i0, i1, i2, indices);
                    AddTriangle(i2, i1, i3, indices);
                }
            }
        }

        vertOffset += (stacks + 1) * (slices + 1);
    }
}

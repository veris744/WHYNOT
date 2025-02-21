#include "Graphics/Mesh.h"

#include "Components/Camera.h"
#include "Material.h"
#include "VertexArray.h"
#include "Managers/World.h"

Mesh::Mesh(const vector<float>& _vertex, const Material& _material)
    : material(make_unique<Material>(_material))
{
    vertexArray = std::make_unique<VertexArray>();
    vertexArray->Bind();
    vertexArray->AddVertexBuffer(_vertex.data(), _vertex.size());
    
    vertexArray->SetLayout(
        {
            LayoutElement("uPos", ShaderDataType::VEC3),
            LayoutElement("uNormal", ShaderDataType::VEC3),
            LayoutElement("uUV", ShaderDataType::VEC2)
        }
    );
}

Mesh::Mesh(const vector<float>& _vertex,const vector<unsigned int>& _index, const Material& _material)
    : material(make_unique<Material>(_material))
{
    
    vertexArray = std::make_unique<VertexArray>();
    vertexArray->Bind();
    vertexArray->AddVertexBuffer(_vertex.data(), _vertex.size());

    vertexArray->SetIndexBuffer(_index.data(), _index.size());
    
    vertexArray->SetLayout(
        {
            LayoutElement("uPos", ShaderDataType::VEC3),
            LayoutElement("uNormal", ShaderDataType::VEC3),
            LayoutElement("uUV", ShaderDataType::VEC2)
        }
    );
}

void Mesh::Render(const mat4& _modelMatrix)
{
    material->BindShader();
    
    material->SetUniforms(_modelMatrix,
        World::GetInstance()->GetCurrentCamera()->GetViewMatrix(),
        World::GetInstance()->GetCurrentCamera()->GetProjectionMatrix(),
         World::GetInstance()->GetCurrentCamera()->GetViewPos());
    
    material->BindTexture();
    vertexArray->Bind();

    if (!vertexArray->GetIndexBuffer())
    {
        vertexArray->DrawVertexBuffers();
    }
    else
    {
        vertexArray->DrawElementBuffer();
    }

    vertexArray->Unbind();
    
}

void Mesh::Clear() const
{
    vertexArray->Unbind();
    material->UnbindShader();
    material->UnbindTexture();
}

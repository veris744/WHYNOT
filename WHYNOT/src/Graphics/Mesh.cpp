#include "Graphics/Mesh.h"

#include "Components/Camera.h"
#include "Material.h"
#include "VertexArray.h"
#include "Managers/World.h"

Mesh::Mesh(const vector<float>& _vertex, const std::shared_ptr<Material>& _material)
    : material(_material)
{
    vertexArray = std::make_shared<VertexArray>();
    vertexArray->Bind();
    vertexArray->AddVertexBuffer(_vertex, _vertex.size() * sizeof(float));
    
    vertexArray->SetLayout(
        {
            LayoutElement("uPos", ShaderDataType::VEC3),
            LayoutElement("uNormal", ShaderDataType::VEC3),
            LayoutElement("uUV", ShaderDataType::VEC2)
        }
    );
}

Mesh::Mesh(const vector<float>& _vertex,const vector<unsigned int>& _index, const std::shared_ptr<Material>& _material)
    : material(_material)
{    
    vertexArray = std::make_shared<VertexArray>();
    vertexArray->Bind();
    vertexArray->AddVertexBuffer(_vertex, _vertex.size() * sizeof(float));

    vertexArray->SetIndexBuffer(_index);
    
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

void Mesh::Clear()
{
    vertexArray->Unbind();
    material->UnbindShader();
    material->UnbindTexture();
}

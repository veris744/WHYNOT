#include "Graphics/Mesh.h"

#include "Components/Camera.h"
#include "Material.h"
#include "VertexArray.h"
#include "Managers/World.h"

Mesh::Mesh(const vector<float>& _vertex, unsigned int _vertexCount, const std::shared_ptr<Material>& _material, unsigned int _elementCount)
    : material(_material)
{
    vertexArray = std::make_unique<VertexArray>();
    vertexArray->Bind();
    vertexArray->AddVertexBuffer(_vertex.data(), _vertexCount, _vertex.size() * sizeof(float));

    if (_elementCount == 3 )
    {
        vertexArray->SetLayout(
            {
                LayoutElement("uPos", ShaderDataType::VEC3),
                LayoutElement("uNormal", ShaderDataType::VEC3),
                LayoutElement("uUV", ShaderDataType::VEC2)
            }
        );
    }
    else if (_elementCount == 2 )
    {
        vertexArray->SetLayout(
            {
                LayoutElement("uPos", ShaderDataType::VEC3),
                LayoutElement("uUV", ShaderDataType::VEC2)
            }
        );
    }
    else
    {
        vertexArray->SetLayout(
            {
                LayoutElement("uPos", ShaderDataType::VEC3)
            }
        );
    }
}

Mesh::Mesh(const vector<float>& _vertex, unsigned int _vertexCount, const vector<unsigned int>& _index, const std::shared_ptr<Material>& _material, unsigned int _elementCount)
    : material(_material)
{
    
    vertexArray = std::make_unique<VertexArray>();
    vertexArray->Bind();
    vertexArray->AddVertexBuffer(_vertex.data(), _vertexCount, _vertex.size() * sizeof(float));

    vertexArray->SetIndexBuffer(_index.data(), _index.size());
    
    vertexArray->SetLayout(
        {
            LayoutElement("uPos", ShaderDataType::VEC3),
            LayoutElement("uNormal", ShaderDataType::VEC3),
            LayoutElement("uUV", ShaderDataType::VEC2)
        }
    );
}

void Mesh::SetLinesVertexArray() const
{
    vertexArray->isLines = true;
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

    if (vertexArray->isLines)
    {
        vertexArray->DrawLinesVertexBuffer();
    }
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

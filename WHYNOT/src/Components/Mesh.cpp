#include "Mesh.h"

#include "Camera.h"
#include "Transform.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/Material.h"
#include "Graphics/VertexArray.h"
#include "Graphics/VertexBuffer.h"
#include "Managers/World.h"

Mesh::Mesh(const vector<float>& _vertex, unsigned int _vertexCount, const std::shared_ptr<Material>& _material)
    : material(_material)
{
    std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(_vertex, _vertex.size() * sizeof(float), _vertexCount);
    vertexArray = std::make_shared<VertexArray>();
    vertexArray->Bind();
    vertexArray->AddVertexBuffer(vertexBuffer);
    
    vertexArray->SetLayout(
        {
            LayoutElement("uPos", ShaderDataType::VEC3),
            LayoutElement("uNormal", ShaderDataType::VEC3),
            LayoutElement("uUV", ShaderDataType::VEC2)
        }
    );
}

Mesh::Mesh(const vector<float>& _vertex, unsigned int _vertexCount, const vector<unsigned int>& _index, unsigned int _indexCount,
    const std::shared_ptr<Material>& _material)
    : material(_material)
{    
    std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(_vertex, _vertex.size() * sizeof(float), _vertexCount);
    vertexArray = std::make_shared<VertexArray>();
    vertexArray->Bind();
    vertexArray->AddVertexBuffer(vertexBuffer);

    std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(_index, _indexCount);
    vertexArray->SetIndexBuffer(indexBuffer);
    
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
        World::GetInstance()->GetCurrentCameraComp()->GetViewMatrix(),
        World::GetInstance()->GetCurrentCameraComp()->GetProjectionMatrix(),
         World::GetInstance()->GetCurrentCameraComp()->GetViewPos());
    
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

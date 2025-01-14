#include "Image2D.h"

#include "Components/Camera.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"
#include "Graphics/VertexArray.h"
#include "Graphics/VertexBuffer.h"
#include "Managers/Renderer2D.h"
#include "Managers/World.h"

Image2D::Image2D(const string& _path, vec2 _pos, vec2 _size)
    : path(_path)
{
    size = _size;
    position = _pos;
    
    vector<float> vertex = Renderer2D::quadVertices;
    unsigned int count = 6;

    vertexArray = std::make_shared<VertexArray>();
    vertexArray->Bind();
    vertexArray->AddVertexBuffer(vertex, vertex.size() * sizeof(float));

    vertexArray->SetLayout(
        {
            LayoutElement("uPos", ShaderDataType::VEC2),
            LayoutElement("uUV", ShaderDataType::VEC2),
        }
    );

    texture = std::make_shared<Texture>(path);
    shader = std::make_shared<Shader>("shaders/vertex2D.glsl", "shaders/fragment2D.glsl");
    shader->Compile();
    shader->Bind();
}

void Image2D::Render()
{
    Widget::Render();

    vertexArray->Bind();
    texture->Bind();
    shader->Bind();
    shader->SetUniformVec2("uPosWidget", position);
    shader->SetUniformVec2("uSize", size);
    shader->SetUniformMat4("uProjection", World::GetInstance()->GetCurrentCameraComp()->GetProjectionMatrix2D());
    if (vertexArray->GetIndexBuffer())
    {
        vertexArray->DrawElementBuffer();        
    }
    else
    {
        vertexArray->DrawVertexBuffers();
    }
}

void Image2D::Clear()
{
    Widget::Clear();
    vertexArray->Unbind();
    shader->Unbind();
}

#include "Image2D.h"

#include "Components/Camera.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"
#include "Graphics/VertexArray.h"
#include "Graphics/VertexBuffer.h"
#include "Managers/Renderer2D.h"
#include "Managers/World.h"


unsigned int Image2D::counter = 0;

Image2D::Image2D(const string& _path, vec2 _pos, vec2 _size, const string& _name)
    : Widget(_pos, _size)
{
    name = _name.empty() ? "Image2D" + std::to_string(++counter) : _name;
    path = _path;
    Initialize();
}

void Image2D::Initialize()
{
    vector<float> vertex = Renderer2D::quadVertices;

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
    shader = std::make_shared<Shader>("shaders/vertex2D.glsl",
        texture->GetNbChannels() == 1 ? "shaders/fragment2DBW.glsl" : "shaders/fragment2D.glsl");
    shader->Compile();
    shader->Bind();
}

void Image2D::Render()
{
    Widget::Render();

    vertexArray->Bind();
    texture->Bind();
    shader->Bind();
    shader->SetUniformVec2("uPosWidget", GetPixelPosition());
    shader->SetUniformVec2("uSize", size);
    
    mat4 projection = glm::ortho(0.0f, Helper::windowWidth, Helper::windowHeight, 0.0f);
    shader->SetUniformMat4("uProjection", projection);
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
    texture->Unbind();
}

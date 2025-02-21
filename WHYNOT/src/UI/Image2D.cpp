#include "Image2D.h"

#include "Components/Camera.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"
#include "Graphics/VertexArray.h"
#include "Graphics/VertexBuffer.h"
#include "Managers/Helper.h"
#include "Managers/Renderer.h"
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
    vertexArray->AddVertexBuffer(vertex.data(), vertex.size());

    vertexArray->SetLayout(
        {
            LayoutElement("uPos", ShaderDataType::VEC2),
            LayoutElement("uUV", ShaderDataType::VEC2),
        }
    );

    texture = Renderer::GetInstance()->GetLoadedTexture(path);
    if (!texture)
    {
       texture = std::make_shared<Texture>(path);
    }
    Renderer::GetInstance()->textures_loaded.push_back(texture);
    
    string shaderNameFrag = "shaders/fragment2D.glsl";
    if (texture->GetNbChannels() == 1)
        shaderNameFrag = "shaders/fragment2DBW.glsl" ;
    if (texture->GetNbChannels() == 2)
        shaderNameFrag = "shaders/fragment2D2Ch.glsl" ;

    string shaderNameVer = "shaders/vertex2D.glsl";
    shader = Renderer::GetInstance()->GetLoadedShader(shaderNameVer, shaderNameFrag);
    if (!shader)
    {
        shader = std::make_shared<Shader>(shaderNameVer, shaderNameFrag);
    }
    Renderer::GetInstance()->shaders_loaded.push_back(shader);
    
    shader->Compile();
    shader->Bind();
}

void Image2D::Render()
{
    Widget::Render();

    if (!vertexArray)
        Initialize();

    vertexArray->Bind();
    texture->Bind();
    shader->Bind();
    shader->SetUniformVec2("uPosWidget", GetPixelPosition());
    shader->SetUniformVec2("uSize", size);
    shader->SetUniformVec3("uColor", color);
    shader->SetUniformFloat("uLayer", 0.1f * layer);
    
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

void Image2D::SetAutoName()
{
    if (name.empty())
        name = "Image2D" + std::to_string(++counter);
}

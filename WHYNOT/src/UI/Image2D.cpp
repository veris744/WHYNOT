#include "Image2D.h"

#include "Components/Camera.h"
#include "Graphics/Material.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"
#include "Graphics/VertexArray.h"
#include "Managers/Helper.h"
#include "Managers/Renderer.h"
#include "Managers/Renderer2D.h"


unsigned int Image2D::counter = 0;

Image2D::Image2D(const string& _path, vec2 _pos, vec2 _size, const string& _name)
    : Widget(_pos, _size)
{
    name = _name.empty() ? "Image2D" + std::to_string(++counter) : _name;
    path = _path;
}

void Image2D::Initialize()
{
    Widget::Initialize();
    
    vector<float> vertex = Renderer2D::quadVertices;
    vertexArray = std::make_unique<VertexArray>();
    vertexArray->Bind();
    vertexArray->AddVertexBuffer(vertex.data(), 0.5f * vertex.size(), vertex.size() * sizeof(float));
    
    vertexArray->SetLayout(
        {
            LayoutElement("uPos", ShaderDataType::VEC2),
            LayoutElement("uUV", ShaderDataType::VEC2),
        }
    );
    
    std::shared_ptr<Texture> texture = Renderer::instance().GetLoadedTexture(path);
    if (!texture)
    {
       texture = std::make_shared<Texture>(path);
    }
    Renderer::instance().textures_loaded.push_back(texture);
    
    string shaderNameFrag = "shaders/fragment2D.glsl";
    if (texture->GetNbChannels() == 1)
        shaderNameFrag = "shaders/fragment2DBW.glsl" ;
    if (texture->GetNbChannels() == 2)
        shaderNameFrag = "shaders/fragment2D2Ch.glsl" ;
    
    string shaderNameVer = "shaders/vertex2D.glsl";
    
    material = std::make_unique<Material>(path.c_str(), shaderNameVer, shaderNameFrag);
}

void Image2D::Render()
{
    Widget::Render();

    if (!vertexArray)
        Initialize();

    vertexArray->Bind();
    
    material->BindTexture();
    material->BindShader();
    material->GetShader()->SetUniformInt("uSampler2D", 0);
    material->GetShader()->SetUniformVec2("uPosWidget", GetPixelPosition());
    material->GetShader()->SetUniformVec2("uSize", size);
    material->GetShader()->SetUniformVec3("uColor", color);
    material->GetShader()->SetUniformFloat("uLayer", 0.1f * layer);
    
    mat4 projection = glm::ortho(0.0f, Helper::windowWidth, Helper::windowHeight, 0.0f);
    material->GetShader()->SetUniformMat4("uProjection", projection);
    
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
    material->UnbindTexture();
    material->UnbindShader();
}

void Image2D::SetAutoName()
{
    if (name.empty())
        name = "Image2D" + std::to_string(++counter);
}

#include "Panel.h"

#include "Graphics/Shader.h"
#include "Managers/Helper.h"
#include "Managers/Renderer2D.h"

unsigned int Panel::counter = 0;

void Panel::Initialize()
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
    
    string shaderNameVer = "shaders/vertex2D.glsl";
    string shaderNameFrag = "shaders/fragment2DColor.glsl";
    
    material = std::make_unique<Material>("", shaderNameVer, shaderNameFrag);
    
    size = GetAutoSize();
}

Panel::Panel(vec2 _pos, vec2 _size, const string& _name)
    : Widget(_pos, _size)
{
    name = _name.empty() ? "Panel" + std::to_string(++counter) : _name;
}

void Panel::Render()
{
    Widget::Render();
    
    if (!vertexArray)
        Initialize();

    vertexArray->Bind();
    
    material->BindTexture();
    material->BindShader();
    material->GetShader()->SetUniformVec2("uPosWidget", GetPixelPosition());
    size = GetAutoSize();
    material->GetShader()->SetUniformVec2("uSize", size);
    material->GetShader()->SetUniformVec4("uColor", background);
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

void Panel::Clear()
{
    Widget::Clear();
    vertexArray->Unbind();
    material->UnbindTexture();
    material->UnbindShader();
}

void Panel::SetAutoName()
{
    if (name.empty())
        name = "Panel" + std::to_string(++counter);
}

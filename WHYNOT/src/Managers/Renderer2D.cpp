#include "Renderer2D.h"

#include "World.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/VertexArray.h"
#include "Graphics/VertexBuffer.h"
#include "UI/Widget.h"


const vector<float> Renderer2D::quadVertices = {
     // Positions    // Texture Coords
     0.0f, 1.0f,   0.0f, 1.0f,  // Top-left
     0.0f, 0.0f,   0.0f, 0.0f,  // Bottom-left
     1.0f, 0.0f,   1.0f, 0.0f,  // Bottom-right

     0.0f, 1.0f,   0.0f, 1.0f,  // Top-left
     1.0f, 0.0f,   1.0f, 0.0f,  // Bottom-right
     1.0f, 1.0f,   1.0f, 1.0f   // Top-right
};


Renderer2D* Renderer2D::instance = nullptr;

Renderer2D* Renderer2D::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new Renderer2D();
    }
    return instance;
}

Renderer2D::Renderer2D()
{
    vertexArray = nullptr;
    defaultShader = nullptr;
}


void Renderer2D::Initialize()
{
    
}

void Renderer2D::Render()
{
    vector<std::shared_ptr<Widget>> widgets = World::GetInstance()->GetWidgets();
    for (const auto& widget : widgets)
    {
        widget->Render();
    }    
}

void Renderer2D::Clear()
{
    vector<std::shared_ptr<Widget>> widgets = World::GetInstance()->GetWidgets();
    for (const auto& widget : widgets)
    {
        widget->Clear();
    }
}
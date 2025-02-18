#include "Renderer2D.h"

#include "World.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/VertexArray.h"
#include "Graphics/VertexBuffer.h"
#include "UI/Widget.h"


const vector<float> Renderer2D::quadVertices = {
    // Positions    // Texture Coords
    -0.5f,  0.5f,   0.0f, 1.0f,  // Top-left
    -0.5f, -0.5f,   0.0f, 0.0f,  // Bottom-left
     0.5f, -0.5f,   1.0f, 0.0f,  // Bottom-right

    -0.5f,  0.5f,   0.0f, 1.0f,  // Top-left
     0.5f, -0.5f,   1.0f, 0.0f,  // Bottom-right
     0.5f,  0.5f,   1.0f, 1.0f   // Top-right
};


std::shared_ptr<Renderer2D> Renderer2D::instance = nullptr;

std::shared_ptr<Renderer2D> Renderer2D::GetInstance()
{
    if (instance == nullptr)
    {
        instance = std::make_shared<Renderer2D>(Renderer2D());
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
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    vector<std::shared_ptr<Widget>> widgets = World::GetInstance()->GetWidgets();
    for (const auto& widget : widgets)
    {
        if (!widget->isActive)  continue;
        widget->Render();
    }    
}

void Renderer2D::Clear()
{
    vector<std::shared_ptr<Widget>> widgets = World::GetInstance()->GetWidgets();
    for (const auto& widget : widgets)
    {
        if (!widget->isActive)  continue;
        widget->Clear();
    }
}

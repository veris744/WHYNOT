#pragma once
#include <memory>
#include <vector>

class Widget;
class Shader;
class VertexArray;

class Renderer2D
{
    Renderer2D();
    
    std::shared_ptr<VertexArray> vertexArray;
    std::shared_ptr<Shader> defaultShader;
    std::vector<std::shared_ptr<Widget>> transparentWidgets;

    void PrepareOpaqueRendering();
    void PrepareTransparentRendering();
    
public:
    static Renderer2D& instance() {
        static Renderer2D INSTANCE;
        return INSTANCE;
    }
    static const std::vector<float> quadVertices;
    
    void Initialize();
    void Render();
    void RenderOpaqueWidget(const std::shared_ptr<Widget>& _widget);
    void Clear();
};

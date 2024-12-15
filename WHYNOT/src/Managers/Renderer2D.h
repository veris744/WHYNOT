#pragma once
#include <includes.h>

class Shader;
class VertexArray;

class Renderer2D
{
    Renderer2D();
    static Renderer2D* instance;
    
    std::shared_ptr<VertexArray> vertexArray;
    std::shared_ptr<Shader> defaultShader;
    
    
public:
    static Renderer2D * GetInstance();
    static const vector<float> quadVertices;
    
    void Initialize();
    void Render();
    void Clear();
};

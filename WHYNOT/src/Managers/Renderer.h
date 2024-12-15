#pragma once
#include <includes.h>

#include "Graphics/Texture.h"

class Material;
class Camera;
class Shader;
class VertexArray;

enum class PrimitiveType
{
    CUBE
};

class Renderer
{
    Renderer();
    static Renderer* instance;
    
public:
    static Renderer * GetInstance();
    static const vector<float> cubeVertex;
    vector<std::shared_ptr<Texture>> textures_loaded; 
    vector<std::shared_ptr<Shader>> shaders_loaded;

    void Initialize();
    void Render();
    void Clear();
    
};

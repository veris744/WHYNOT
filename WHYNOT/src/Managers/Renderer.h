#pragma once
#include <includes.h>

#include "Graphics/Texture.h"

class Material;
class Camera;
class Shader;
class VertexArray;

enum class PrimitiveType
{
    CUBE, SPHERE
};

class Renderer
{
    Renderer();
    static Renderer* instance;
    static const vector<float> cubeVertex;
    static vector<float> sphereVertex;
    static vector<unsigned int> sphereIndex;
    
public:
    static Renderer * GetInstance();
    vector<std::shared_ptr<Texture>> textures_loaded; 
    vector<std::shared_ptr<Shader>> shaders_loaded;

    void Initialize();
    void Render();
    void Clear();

    static const vector<float> GetCubeVertex() { return cubeVertex; }
    static const vector<float> GetSphereVertex() { return sphereVertex; }
    static const vector<unsigned int>& GetShereIndex() { return sphereIndex; }
    static void SetSphereVertex(float radius, int sectors, int stacks)
    {
        sphereVertex.clear();
        sphereIndex.clear();
        Helper::generateSphere(sphereVertex, sphereIndex, radius, sectors, stacks);
    }
    
};

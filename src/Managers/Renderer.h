#pragma once

#include "Helper.h"
#include "Reflection/Reflection.h"

class Texture;
class Camera;
class Shader;
class VertexArray;

enum class PrimitiveType
{
    NONE, BOX, SPHERE
};
REGISTER_ENUM(PrimitiveType,
    {"NONE", PrimitiveType::NONE},
    {"BOX", PrimitiveType::BOX},
    {"SPHERE", PrimitiveType::SPHERE}
);

class Renderer
{
    Renderer();
    static const vector<float> cubeVertex;
    static vector<float> sphereVertex;
    static vector<unsigned int> sphereIndex;
    
public:
    static Renderer& instance() {
        static Renderer INSTANCE;
        return INSTANCE;
    }
    vector<std::shared_ptr<Texture>> textures_loaded; 
    vector<std::shared_ptr<Shader>> shaders_loaded;

    void Initialize();
    void Render();
    void Clear();
    void CleanUp();

    const std::shared_ptr<Texture>& GetLoadedTexture(const string& path) const;
    const std::shared_ptr<Shader>& GetLoadedShader(const string& pathVertex, const string& pathFragment) const;

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

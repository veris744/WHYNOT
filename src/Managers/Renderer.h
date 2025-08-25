#pragma once

#include "Helper.h"
#include "Reflection/Reflection.h"

class Texture;
class Camera;
class Shader;
class VertexArray;

enum class PrimitiveType
{
    NONE, BOX, SPHERE, PLANE, CAPSULE, SLOPE
};
REGISTER_ENUM(PrimitiveType,
    {"NONE", PrimitiveType::NONE},
    {"BOX", PrimitiveType::BOX},
    {"SPHERE", PrimitiveType::SPHERE},
    {"PLANE", PrimitiveType::PLANE},
    {"CAPSULE", PrimitiveType::CAPSULE},
    {"SLOPE", PrimitiveType::SLOPE}
);

class Renderer
{
    Renderer();
    static const std::vector<float> cubeVertex;
    static const std::vector<float> planeVertex;
    static const std::vector<float> slopeVertex;
    static std::vector<float> sphereVertex;
    static std::vector<unsigned int> sphereIndex;
    
public:
    static Renderer& instance() {
        static Renderer INSTANCE;
        return INSTANCE;
    }
    std::vector<std::shared_ptr<Texture>> textures_loaded; 
    std::vector<std::shared_ptr<Shader>> shaders_loaded;

    void Initialize();
    void Render();
    void Clear();
    void CleanUp();

    const std::shared_ptr<Texture>& GetLoadedTexture(const std::string& path) const;
    const std::shared_ptr<Shader>& GetLoadedShader(const std::string& pathVertex, const std::string& pathFragment) const;

    static const std::vector<float> GetCubeVertex() { return cubeVertex; }
    static const std::vector<float> GetPlaneVertex() { return planeVertex; }
    static const std::vector<float> GetSlopeVertex() { return slopeVertex; }
    static const std::vector<float> GetSphereVertex() { return sphereVertex; }
    static const void GenerateCapsuleVertex(float radius, float height, std::vector<float>& vertices, std::vector<unsigned int>& indices);
    static const std::vector<unsigned int>& GetSphereIndex() { return sphereIndex; }
    static void SetSphereVertex(float radius, int sectors, int stacks)
    {
        sphereVertex.clear();
        sphereIndex.clear();
        Helper::generateSphere(sphereVertex, sphereIndex, radius, sectors, stacks);
    }
    
};

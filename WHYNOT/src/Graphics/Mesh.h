#pragma once
#include <includes.h>

#include "VertexArray.h"
#include "Reflection/Reflection.h"

class Camera;
class Material;
class VertexArray;

class Mesh
{
    std::unique_ptr<VertexArray> vertexArray;
    std::shared_ptr<Material> material;

public:
    Mesh(const vector<float>& _vertex, const std::shared_ptr<Material>& _material);
    Mesh(const vector<float>& _vertex, const vector<unsigned int>& _index,
         const std::shared_ptr<Material>& _material);

    ~Mesh()
    {
        vertexArray = nullptr;
        material = nullptr;
    }
    
    void Render(const mat4& _modelMatrix);
    void Clear() const;
    Material* GetMaterial() const { return material.get(); }
};
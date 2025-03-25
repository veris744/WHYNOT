#pragma once

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
    Mesh(const vector<float>& _vertex, unsigned int _vertexCount, const std::shared_ptr<Material>& _material, unsigned int _elementCount = 3);
    Mesh(const vector<float>& _vertex, unsigned int _vertexCount, const vector<unsigned int>& _index,
         const std::shared_ptr<Material>& _material, unsigned int _elementCount = 3);

    ~Mesh()
    {
        vertexArray = nullptr;
        material = nullptr;
    }
    void SetLinesVertexArray() const;
    void Render(const mat4& _modelMatrix);
    void Clear() const;
    Material* GetMaterial() const { return material.get(); }
};
#pragma once
#include <memory>

#include "Component.h"

class Camera;
class Material;
class VertexArray;

class Mesh
{
    std::shared_ptr<VertexArray> vertexArray;
    std::shared_ptr<Material> material;

    
public:
    Mesh(const vector<float>& _vertex, unsigned int _vertexCount, const std::shared_ptr<Material>& _material);
    Mesh(const vector<float>& _vertex, unsigned int _vertexCount, const vector<unsigned int>& _index,
         unsigned int _indexCount,
         const std::shared_ptr<Material>& _material);
    
    void Render(const mat4& _modelMatrix);
    void Clear();
};

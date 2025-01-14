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
    Mesh(const vector<float>& _vertex, const std::shared_ptr<Material>& _material);
    Mesh(const vector<float>& _vertex, const vector<unsigned int>& _index,
         const std::shared_ptr<Material>& _material);
    
    void Render(const mat4& _modelMatrix);
    void Clear();
    std::shared_ptr<Material> GetMaterial() const { return material; }
};

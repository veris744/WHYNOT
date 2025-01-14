#pragma once
#include <includes.h>

class VertexBuffer
{
    vector<float> vertex;
    unsigned int vertexCount;
    unsigned int size;
    
public:
    VertexBuffer(const vector<float>& i_vertex, unsigned int i_size, unsigned int i_count);

    void Bind();
    void Unbind();
    unsigned int VBO;

    const vector<float>& GetVertex() const { return vertex; }
    unsigned int GetCount() const { return vertexCount; }
    unsigned int GetSize() const { return size; }
    
};

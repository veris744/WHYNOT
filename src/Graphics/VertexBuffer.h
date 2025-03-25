#pragma once

class VertexBuffer
{
    unsigned int vertexCount;
    unsigned int size;
    
public:
    VertexBuffer(const float* _vertex, unsigned int _count, unsigned int _size);
    ~VertexBuffer();

    void Bind();
    void Unbind();
    unsigned int VBO;

    unsigned int GetCount() const { return vertexCount; }
    unsigned int GetSize() const { return size; }
    
};

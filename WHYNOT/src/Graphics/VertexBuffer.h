#pragma once
#include <includes.h>

class VertexBuffer
{
    unsigned int vertexCount;
    unsigned int size;
    
public:
    VertexBuffer(const float* _vertex, unsigned int _count, unsigned int _size);
    ~VertexBuffer()
    {
        Unbind();
    
        if (VBO != 0)
        {
            glDeleteBuffers(1, &VBO);
            VBO = 0;
        }
    }

    void Bind();
    void Unbind();
    unsigned int VBO;

    unsigned int GetCount() const { return vertexCount; }
    unsigned int GetSize() const { return size; }
    
};

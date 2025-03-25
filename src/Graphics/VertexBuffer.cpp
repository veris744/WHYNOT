#include "VertexBuffer.h"
#include <glad/include/glad/glad.h>

VertexBuffer::VertexBuffer(const float* _vertex, unsigned int _count, unsigned int _size)
    : vertexCount(_count), size(_size)
{
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, _vertex, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
    Unbind();
    
    if (VBO != 0)
    {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }
}

void VertexBuffer::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
}

void VertexBuffer::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


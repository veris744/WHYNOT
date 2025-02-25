#include "VertexBuffer.h"
#include <includes.h>

VertexBuffer::VertexBuffer(const float* _vertex, unsigned int _count, unsigned int _size)
    : vertexCount(_count), size(_size)
{
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, _vertex, GL_STATIC_DRAW);
}

void VertexBuffer::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
}

void VertexBuffer::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


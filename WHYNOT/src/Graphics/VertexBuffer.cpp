#include "VertexBuffer.h"
#include <includes.h>

VertexBuffer::VertexBuffer(const vector<float>& _vertex, unsigned int _size, unsigned int _count)
    : vertex(_vertex)
    , vertexCount(_count)
    , size(_size)
{
    glGenBuffers(1, &VBO);
}

void VertexBuffer::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertex.data(), GL_STATIC_DRAW);
}

void VertexBuffer::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

#include "VertexBuffer.h"
#include <includes.h>

VertexBuffer::VertexBuffer(const vector<float>& i_vertex, unsigned int i_size, unsigned int i_count)
    : vertex(i_vertex)
    , vertexCount(i_count)
    , size(i_size)
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

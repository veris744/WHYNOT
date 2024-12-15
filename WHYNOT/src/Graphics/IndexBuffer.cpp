#include "IndexBuffer.h"
#include <includes.h>

IndexBuffer::IndexBuffer(const vector<unsigned int>& _index, unsigned int _count)
    : index(_index),
    count(_count)
{
    glGenBuffers(1, &id);
}

void IndexBuffer::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), index.data(), GL_STATIC_DRAW);
}

void IndexBuffer::Unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

unsigned int IndexBuffer::GetCount()
{
    return count;
}

const vector<unsigned int>& IndexBuffer::GetIndex()
{
    return index;
}

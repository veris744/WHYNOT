#include "IndexBuffer.h"
#include <glad/include/glad/glad.h>

IndexBuffer::IndexBuffer(const unsigned int* _index, unsigned int _count)
    : count(_count)
{
    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), _index, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
    Unbind();
    
    if (id != 0)
    {
        glDeleteBuffers(1, &id);
        id = 0;
    }
}

void IndexBuffer::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void IndexBuffer::Unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

unsigned int IndexBuffer::GetCount() const
{
    return count;
}

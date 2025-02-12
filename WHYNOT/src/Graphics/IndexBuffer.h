#pragma once
#include <includes.h>

class IndexBuffer
{
    unsigned int count;
    
public:
    IndexBuffer(const unsigned int* _index, unsigned int _count);
    ~IndexBuffer()
    {
        Unbind();
    
        if (id != 0)
        {
            glDeleteBuffers(1, &id);
            id = 0;
        }
    }

    unsigned int id;

    void Bind();
    void Unbind();
    unsigned int GetCount() const;
};

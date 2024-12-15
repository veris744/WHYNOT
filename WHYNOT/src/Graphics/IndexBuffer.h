#pragma once
#include <includes.h>

class IndexBuffer
{
    vector<unsigned int> index;
    unsigned int count;
    
public:
    IndexBuffer(const vector<unsigned int>& _index, unsigned int _count);
    ~IndexBuffer() = default;

    unsigned int id;

    void Bind();
    void Unbind();
    unsigned int GetCount();
    const vector<unsigned int>& GetIndex();
};

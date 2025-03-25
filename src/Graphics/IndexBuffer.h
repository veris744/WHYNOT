#pragma once

class IndexBuffer
{
    unsigned int count;
    
public:
    IndexBuffer(const unsigned int* _index, unsigned int _count);
    ~IndexBuffer();

    unsigned int id;

    void Bind();
    void Unbind();
    unsigned int GetCount() const;
};

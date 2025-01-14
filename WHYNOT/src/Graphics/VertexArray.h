#pragma once
#include "includes.h"
#include "IndexBuffer.h"
#include "LayoutElement.h"
#include "VertexBuffer.h"


class VertexArray
{
    vector<std::unique_ptr<VertexBuffer>> vertexBuffers;
    std::unique_ptr<IndexBuffer> indexBuffer;
    
public:
    VertexArray();
    
    unsigned int VAO;

    void Bind();
    void Unbind();
    void AddVertexBuffer(const vector<float>& _vertex, unsigned int _size);
    void SetIndexBuffer(const vector<unsigned int>& _index);
    const std::unique_ptr<IndexBuffer>& GetIndexBuffer() const { return indexBuffer; }
    void BindVertexBuffers();
    void BindIndexBuffer();
    void DrawVertexBuffers();
    void DrawElementBuffer();
    void SetLayout(const vector<LayoutElement>& _elements);
    
};

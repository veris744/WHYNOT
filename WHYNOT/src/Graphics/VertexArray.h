#pragma once
#include "includes.h"
#include "LayoutElement.h"

class IndexBuffer;

class VertexBuffer;

class VertexArray
{
    vector<std::shared_ptr<VertexBuffer>> vertexBuffers;
    std::shared_ptr<IndexBuffer> indexBuffer;
    
public:
    VertexArray();
    ~VertexArray() = default;
    
    unsigned int VAO;

    void Bind();
    void Unbind();
    void AddVertexBuffer(std::shared_ptr<VertexBuffer>& _vertexBuffer);
    void SetIndexBuffer(std::shared_ptr<IndexBuffer>& _indexBuffer);
    const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return indexBuffer; }
    void BindVertexBuffers();
    void BindIndexBuffer();
    void DrawVertexBuffers();
    void DrawElementBuffer();
    void SetLayout(const vector<LayoutElement>& _elements);
    
};

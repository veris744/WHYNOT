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
    ~VertexArray()
    {
        indexBuffer = nullptr;
        vertexBuffers.clear();
    
        Unbind();
        glDeleteVertexArrays(1, &VAO);
    }
    
    unsigned int VAO;
    bool isLines = false;

    void Bind();
    void Unbind();
    void AddVertexBuffer(const float* _vertex, unsigned int _count, unsigned int _size);
    void SetIndexBuffer(const unsigned int* _index, unsigned int _count);
    const std::unique_ptr<IndexBuffer>& GetIndexBuffer() const { return indexBuffer; }
    void BindVertexBuffers();
    void BindIndexBuffer();
    void DrawVertexBuffers();
    void DrawElementBuffer();
    void DrawLinesVertexBuffer();
    void SetLayout(const vector<LayoutElement>& _elements);
    
};

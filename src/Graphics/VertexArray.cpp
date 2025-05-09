#include "VertexArray.h"

#include "IndexBuffer.h"

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &VAO);
}

void VertexArray::Bind()
{
    glBindVertexArray(VAO);
}

void VertexArray::Unbind()
{
    glBindVertexArray(0);
}

void VertexArray::AddVertexBuffer(const float* _vertex, unsigned int _count, unsigned int _size)
{
    std::unique_ptr<VertexBuffer> buff = std::make_unique<VertexBuffer>(_vertex, _count, _size);
    vertexBuffers.push_back(std::move(buff));
    vertexBuffers.back()->Bind();
}

void VertexArray::SetIndexBuffer(const unsigned int* _index, unsigned int _count)
{
    indexBuffer = std::make_unique<IndexBuffer>(_index, _count);
    indexBuffer->Bind();
}

void VertexArray::BindVertexBuffers()
{
    for (const auto& buffer : vertexBuffers)
    {
        buffer->Bind(); 
    }
}

void VertexArray::BindIndexBuffer()
{
    indexBuffer->Bind();
}

void VertexArray::DrawVertexBuffers()
{
    unsigned int count = vertexBuffers.front()->GetCount();
	glDrawArrays(GL_TRIANGLES, 0, count);
}

void VertexArray::DrawElementBuffer()
{
	glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, 0);
}

void VertexArray::DrawLinesVertexBuffer()
{
    glLineWidth(3);
    glDrawArrays(GL_LINES, 0, vertexBuffers.front()->GetCount());
}

void VertexArray::SetLayout(const vector<LayoutElement>& _elements)
{
    int i = 0;
    unsigned int stride = 0;
    for (const auto& element : _elements)
    {
        stride += element.size;
    }

    unsigned int offset = 0;
    for (LayoutElement element : _elements)
    {
        element.index = i;
        element.offset = offset;
        element.stride = stride;
        element.SetLayoutElement();

        offset += element.size;
        i++;
    }
}



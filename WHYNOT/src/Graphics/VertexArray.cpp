#include "VertexArray.h"

#include "LayoutElement.h"

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
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void VertexArray::AddVertexBuffer(const vector<float>& _vertex, unsigned int _size)
{
    std::unique_ptr<VertexBuffer> buff = std::make_unique<VertexBuffer>(_vertex, _size, _vertex.size());
    vertexBuffers.push_back(std::move(buff));
    vertexBuffers.back()->Bind();
}

void VertexArray::SetIndexBuffer(const vector<unsigned int>& _index)
{
    indexBuffer = std::make_unique<IndexBuffer>(_index, _index.size());
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

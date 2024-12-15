#include "VertexArray.h"

#include "IndexBuffer.h"
#include "LayoutElement.h"
#include "VertexBuffer.h"

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

void VertexArray::AddVertexBuffer(std::shared_ptr<VertexBuffer>& _vertexBuffer)
{
    vertexBuffers.push_back(_vertexBuffer);
    _vertexBuffer->Bind();
}

void VertexArray::SetIndexBuffer(std::shared_ptr<IndexBuffer>& _indexBuffer)
{
    indexBuffer = _indexBuffer;
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

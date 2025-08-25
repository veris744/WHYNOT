#pragma once
#include <glad/include/glad/glad.h>
#include <string>


enum class ShaderDataType
{
    BOOL, INT, FLOAT, SAMPLER2D, VEC2, VEC3
};


struct LayoutElement
{
    unsigned int index;
    const std::string name;
    unsigned int size;
    unsigned int offset;
    unsigned int stride;
    bool normalized;
    unsigned int type;
    unsigned int count;

    LayoutElement() = delete;
    LayoutElement(const char* _name, ShaderDataType _type, bool _normalized = false)
        :  index(0), name(_name), offset(0), stride(0), normalized(_normalized) 
    {
        SetLayoutElementData(_type);
    }

    ~LayoutElement() = default;

    void SetLayoutElementData(ShaderDataType _type)
    {
        switch (_type)
        {
            case ShaderDataType::BOOL:
                type = GL_BOOL;
                size = sizeof(bool);
                count = 1;
                break;
            case ShaderDataType::INT:
                type = GL_INT;
                size = sizeof(int);
                count = 1;
                break;
            case ShaderDataType::FLOAT:
                type = GL_FLOAT;
                size = sizeof(float);
                count = 1;
                break;
            case ShaderDataType::SAMPLER2D:
                type = GL_INT;
                size = sizeof(int);
                count = 1;
                break;
            case ShaderDataType::VEC2:
                type = GL_FLOAT;
                size = sizeof(float) * 2;
                count = 2;
                break;
            case ShaderDataType::VEC3:
                type = GL_FLOAT;
                size = sizeof(float) * 3;
                count = 3;
                break;
        }
    }

    void SetLayoutElement() const
    {
        glVertexAttribPointer(index, count, type, normalized ? GL_TRUE : GL_FALSE, stride, (void*)offset);
        glEnableVertexAttribArray(index);
    }
};

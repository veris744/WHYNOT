#pragma once
#include "Widget.h"

class Shader;
class Texture;
class VertexArray;

class Image2D : public Widget
{
    string path;
    std::shared_ptr<VertexArray> vertexArray;
    std::shared_ptr<Texture> texture;
    std::shared_ptr<Shader> shader;
    
public:
    Image2D(const string& _path, vec2 _pos, vec2 _size);
    void Render() override;
    void Clear() override;
};

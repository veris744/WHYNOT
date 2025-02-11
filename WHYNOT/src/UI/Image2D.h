#pragma once
#include "Widget.h"

class Shader;
class Texture;
class VertexArray;

class Image2D : public Widget
{
    static unsigned int counter;
    string path;
    std::shared_ptr<VertexArray> vertexArray;
    std::shared_ptr<Texture> texture;
    std::shared_ptr<Shader> shader;

    void Initialize();
    
public:
    Image2D(const string& _path, vec2 _pos, vec2 _size, const string& _name = "");
    void Render() override;
    void Clear() override;

    vec3 color = vec3(0, 0, 0);
};

#pragma once
#include "Widget.h"

class Shader;
class Texture;
class VertexArray;

class Image2D : public Widget
{
    static unsigned int counter;
    std::shared_ptr<VertexArray> vertexArray;
    std::shared_ptr<Texture> texture;
    std::shared_ptr<Shader> shader;

    void Initialize();
    
public:
    Image2D(const string& _path, vec2 _pos, vec2 _size, const string& _name = "");
    Image2D(){};
    void Render() override;
    void Clear() override;
    void SetAutoName() override;

    string path = "";
    vec3 color = vec3(0, 0, 0);
};
REGISTER_CLASS(Image2D, {
    REGISTER_MEMBER(Image2D, name),
    REGISTER_MEMBER(Image2D, path),
    REGISTER_MEMBER(Image2D, color),
    REGISTER_MEMBER(Image2D, position),
    REGISTER_MEMBER(Image2D, size),
    REGISTER_MEMBER(Image2D, layer)
})
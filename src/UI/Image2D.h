#pragma once
#include "Widget.h"
#include "Graphics/Material.h"
#include "Graphics/VertexArray.h"

class Image2D : public Widget
{
    static unsigned int counter;
    std::unique_ptr<VertexArray> vertexArray;
    std::unique_ptr<Material> material;

    void Initialize() override;
    
public:
    Image2D(const std::string& _path, glm::vec2 _pos, glm::vec2 _size, const std::string& _name = "");
    Image2D() = default;
    void Render() override;
    void Clear() override;
    void SetAutoName() override;

    std::string path = "";
    glm::vec3 color = glm::vec3(0, 0, 0);
    float rotation = 0.0f;
};
REGISTER_CLASS(Image2D, {
    REGISTER_MEMBER(Image2D, name, MemberProperty::Viewable),
    REGISTER_MEMBER(Image2D, path, MemberProperty::Viewable),
    REGISTER_MEMBER(Image2D, color, MemberProperty::Viewable),
    REGISTER_MEMBER(Image2D, position, MemberProperty::Viewable),
    REGISTER_MEMBER(Image2D, size, MemberProperty::Viewable),
    REGISTER_MEMBER(Image2D, layer, MemberProperty::Viewable),
    REGISTER_MEMBER(Image2D, hasTransparency, MemberProperty::Viewable),
    REGISTER_MEMBER(Image2D, autoSizing, MemberProperty::Viewable),
    REGISTER_MEMBER(Image2D, pixelCorrection, MemberProperty::Viewable),
    REGISTER_MEMBER(Image2D, rotation, MemberProperty::Viewable)
})
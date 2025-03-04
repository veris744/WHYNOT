#pragma once
#include "UI/Widget.h"
#include "Graphics/Material.h"
#include "Graphics/VertexArray.h"


class Panel : public Widget
{
    static unsigned int counter;
    
protected:
    std::unique_ptr<VertexArray> vertexArray;
    std::unique_ptr<Material> material;

    void Initialize() override;
    
public:
    Panel(vec2 _pos, vec2 _size, const string& _name = "");
    Panel() = default;
    void Render() override;
    void Clear() override;
    void SetAutoName() override;

    vec4 background = vec4(0, 0, 0, 0);
};
REGISTER_CLASS(Panel, {
    REGISTER_MEMBER(Panel, name, MemberProperty::Viewable),
    REGISTER_MEMBER(Panel, background, MemberProperty::Viewable),
    REGISTER_MEMBER(Panel, position, MemberProperty::Viewable),
    REGISTER_MEMBER(Panel, size, MemberProperty::Viewable),
    REGISTER_MEMBER(Panel, layer, MemberProperty::Viewable),
    REGISTER_MEMBER(Panel, hasTransparency, MemberProperty::Viewable),
    REGISTER_MEMBER(Panel, autoSizing, MemberProperty::Viewable),
    REGISTER_MEMBER(Panel, pixelCorrection, MemberProperty::Viewable)
})
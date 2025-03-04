#pragma once
#include "Panel.h"

class Entity;
class EntityView;

class EntityPanel : public Panel
{
    void Initialize() override;
    Entity * entity = nullptr;
    
public:
    EntityPanel(vec2 _pos, vec2 _size, const string& _name = "");
    EntityPanel() = default;
    
    void SetEntity(Entity * _entity) { entity = _entity; }
    void SetContent();
    void ClearContent();
    
};
REGISTER_CLASS(EntityPanel, {
    REGISTER_MEMBER(EntityPanel, name, MemberProperty::Viewable),
    REGISTER_MEMBER(EntityPanel, background, MemberProperty::Viewable),
    REGISTER_MEMBER(EntityPanel, position, MemberProperty::Viewable),
    REGISTER_MEMBER(EntityPanel, size, MemberProperty::Viewable),
    REGISTER_MEMBER(EntityPanel, layer, MemberProperty::Viewable),
    REGISTER_MEMBER(EntityPanel, hasTransparency, MemberProperty::Viewable),
    REGISTER_MEMBER(EntityPanel, autoSizing, MemberProperty::Viewable),
    REGISTER_MEMBER(EntityPanel, pixelCorrection, MemberProperty::Viewable)
})

#pragma once
#include "Panel.h"

class EntityView;

class EntityPanel : public Panel
{
    void Initialize() override;
    
public:
    EntityPanel(vec2 _pos, vec2 _size, const string& _name = "");
    EntityPanel() = default;
    
};
REGISTER_CLASS(EntityPanel, {
    REGISTER_MEMBER(EntityPanel, name),
    REGISTER_MEMBER(EntityPanel, background),
    REGISTER_MEMBER(EntityPanel, position),
    REGISTER_MEMBER(EntityPanel, size),
    REGISTER_MEMBER(EntityPanel, layer),
    REGISTER_MEMBER(EntityPanel, hasTransparency),
    REGISTER_MEMBER(EntityPanel, autoSizing),
    REGISTER_MEMBER(EntityPanel, padding)
})

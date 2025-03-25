#pragma once
#include "Panel.h"

class Entity;
class EntityView;

class EntityPanel : public Panel
{
    void Initialize() override;
    Entity * entity = nullptr;

    float separation = 20.f;
    float panelHeight = 0.f;
    
public:
    EntityPanel(vec2 _pos, vec2 _size, const string& _name = "");
    EntityPanel() = default;
    
    void SetEntity(Entity * _entity) { entity = _entity; }
    void SetContent();
    void CreateMemberView(ReflectedObject* _object, const TypeInfo& _typeInfo, float _margin);
    void ClearContent();
    
};

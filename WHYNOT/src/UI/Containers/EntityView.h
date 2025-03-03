#pragma once
#include "Entities/Entity.h"
#include "UI/Widget.h"

class EntityView : public Widget
{
    static unsigned int counter;
    Entity * entity = nullptr;
    
public:
    EntityView(vec2 _pos, vec2 _size, const string& _name = "");
    EntityView() = default;
    void SetAutoName() override;
    void Render() override;

    void SetEntity(Entity * _entity) { entity = _entity; }
    void SetContent();

    void ClearContent();
};

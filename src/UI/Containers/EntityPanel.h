#pragma once
#include "Dropbox.h"
#include "Panel.h"

class Entity;
class EntityView;

class EntityPanel : public Panel
{
    void Initialize() override;
    Entity * entity = nullptr;
    std::vector<Widget*> entityViews;
    
public:
    EntityPanel(vec2 _pos, vec2 _size, const string& _name = "");
    EntityPanel() = default;

    void SetContent(Entity* _entity);
    void SetContent();
    void ResetContent();
    void CreateMemberView(ReflectedObject* _object, const TypeInfo& _typeInfo, float _margin, Dropbox* _dropbox = nullptr);
    void ClearContent();
    void ShowEntities(bool _areShown);
    void ClearPropertiesContent();

    void Reorganize();

};

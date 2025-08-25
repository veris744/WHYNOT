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
    EntityPanel(glm::vec2 _pos, glm::vec2 _size, const std::string& _name = "");
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

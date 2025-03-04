#include "EntityPanel.h"

#include "EntityView.h"


EntityPanel::EntityPanel(vec2 _pos, vec2 _size, const string& _name)
    : Panel(_pos, _size, _name)
{
    
}

void EntityPanel::Initialize()
{
    Panel::Initialize();

    std::shared_ptr<EntityView> entityView = std::make_shared<EntityView>();
    entityView->name = "EditorEntityView";
    entityView->autoSizing = AutoSizing::ALL;
    AddWidget(entityView);
}


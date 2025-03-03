#include "EntityView.h"

#include "MemberView.h"
#include "UI/Text.h"

unsigned int EntityView::counter = 0;

EntityView::EntityView(vec2 _pos, vec2 _size, const string& _name)
    : Widget(_pos, _size)
{
    name = _name.empty() ? "EntityView" + std::to_string(++counter) : _name;
}

void EntityView::SetAutoName()
{
    if (name.empty())
        name = "EntityView" + std::to_string(++counter);
}

void EntityView::Render()
{
    if (!entity) return;
    
    Widget::Render();
}

void EntityView::SetContent()
{
    if (!entity) return;

    float padding = 0;
    float separation = 25;
    
    std::shared_ptr<MemberView> entityView = std::make_shared<MemberView>(vec2(0), vec2(0, 20), "EntityNameText");
    entityView->autoSizing = AutoSizing::HORIZONTAL;
    entityView->padding = {250, 0, padding, -100};
    AddWidget(entityView);
    entityView->SetMemberInfo(entity->GetName());
    
    for (const auto& comp : entity->GetComponents())
    {
        padding += separation;
        const auto* typeInfo = TypeRegistry::instance().getTypeInfo(Reader::demangleTypeName(typeid(*comp.get()).name()));
        if (!typeInfo)  continue;
        
        std::shared_ptr<MemberView> compView = std::make_shared<MemberView>(vec2(0), vec2(0, 20));
        compView->autoSizing = AutoSizing::HORIZONTAL;
        compView->padding = {250, 0, padding, -100};
        AddWidget(compView);
        compView->SetMemberInfo(typeInfo->type_name);
        
        for (const auto& member : typeInfo->members)
        {
            padding += separation;
            std::shared_ptr<MemberView> memberView = std::make_shared<MemberView>(vec2(0), vec2(0, 20));
            memberView->autoSizing = AutoSizing::HORIZONTAL;
            memberView->padding = {250, 0, padding, -100};
            AddWidget(memberView);
            memberView->SetMemberInfo(member.name);
        }
    }
}

void EntityView::ClearContent()
{
    entity = nullptr;
    children.clear();
}

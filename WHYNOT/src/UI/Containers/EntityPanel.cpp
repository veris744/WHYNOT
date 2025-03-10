#include "EntityPanel.h"

#include "MemberView.h"
#include "Entities/Entity.h"


EntityPanel::EntityPanel(vec2 _pos, vec2 _size, const string& _name)
    : Panel(_pos, _size, _name)
{
    
}

void EntityPanel::Initialize()
{
    Panel::Initialize();
    isActive = false;
}


void EntityPanel::SetContent()
{
    if (!entity) return;
    isActive = true;

    float yCorrection = 10;
    float separation = 20;
    
    std::shared_ptr<MemberView> entityView = std::make_shared<MemberView>(vec2(0, -50), vec2(0, 20), "EntityNameText");
    entityView->autoSizing = AutoSizing::HORIZONTAL;
    entityView->pixelCorrection = {10, yCorrection};
    AddWidget(entityView);
    entityView->SetMemberInfo(entity->GetName());
    yCorrection += 8;
    
    for (const auto& comp : entity->GetComponents())
    {
        yCorrection += separation;
        const auto* typeInfo = TypeRegistry::instance().getTypeInfo(Reader::demangleTypeName(typeid(*comp.get()).name()));
        if (!typeInfo)  continue;
        
        std::shared_ptr<MemberView> compView = std::make_shared<MemberView>(vec2(0, -50), vec2(0, 20));
        compView->autoSizing = AutoSizing::HORIZONTAL;
        compView->pixelCorrection = {10, yCorrection};
        AddWidget(compView);
        compView->SetMemberInfo(typeInfo->type_name);
        
        for (const auto& member : typeInfo->members)
        {
            yCorrection += separation;
            std::shared_ptr<MemberView> memberView = std::make_shared<MemberView>(vec2(0, -50), vec2(0, 20));
            memberView->autoSizing = AutoSizing::HORIZONTAL;
            memberView->pixelCorrection = {10, yCorrection};
            AddWidget(memberView);
            memberView->SetMemberInfo(member, comp.get());
        }
        yCorrection += 8;
    }
}

void EntityPanel::ClearContent()
{
    entity = nullptr;
    children.clear();
    isActive = false;
}


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
    
    size = GetAutoSize();
    Widget::Render();
}

void EntityView::SetContent()
{
    if (!entity) return;

    float yCorrection = 0;
    float separation = 20;
    
    std::shared_ptr<MemberView> entityView = std::make_shared<MemberView>(vec2(-45, -20), vec2(0, 20), "EntityNameText");
    entityView->autoSizing = AutoSizing::HORIZONTAL;
    entityView->pixelCorrection = {0, yCorrection};
    AddWidget(entityView);
    entityView->SetMemberInfo(entity->GetName());
    
    for (const auto& comp : entity->GetComponents())
    {
        yCorrection += separation;
        const auto* typeInfo = TypeRegistry::instance().getTypeInfo(Reader::demangleTypeName(typeid(*comp.get()).name()));
        if (!typeInfo)  continue;
        
        std::shared_ptr<MemberView> compView = std::make_shared<MemberView>(vec2(-40, -20), vec2(0, 20));
        compView->autoSizing = AutoSizing::HORIZONTAL;
        compView->pixelCorrection = {0, yCorrection};
        AddWidget(compView);
        compView->SetMemberInfo(typeInfo->type_name);
        
        for (const auto& member : typeInfo->members)
        {
            yCorrection += separation;
            std::shared_ptr<MemberView> memberView = std::make_shared<MemberView>(vec2(-35, -20), vec2(0, 20));
            memberView->autoSizing = AutoSizing::HORIZONTAL;
            memberView->pixelCorrection = {0, yCorrection};
            AddWidget(memberView);
            std::string valueStr;
            if (member.getter) {
                valueStr = member.getter(comp.get());
            }
            memberView->SetMemberInfo(member, valueStr);
        }
    }
}

void EntityView::ClearContent()
{
    entity = nullptr;
    children.clear();
}

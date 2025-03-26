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

    panelHeight = 10;

    
    std::shared_ptr<MemberView> entityView = std::make_shared<MemberView>(vec2(0, -50), vec2(0, 20), "EntityNameText");
    entityView->autoSizing = AutoSizing::HORIZONTAL;
    entityView->pixelCorrection = {15, panelHeight};
    AddWidget(entityView);
    entityView->SetMemberInfo(entity->GetName());
    panelHeight += separation;
    
    for (const auto& comp : entity->GetComponents())
    {
        const auto* typeInfo = TypeRegistry::instance().getTypeInfo(Reader::demangleTypeName(typeid(*comp.get()).name()));
        if (!typeInfo)  continue;
        
        std::shared_ptr<MemberView> compView = std::make_shared<MemberView>(vec2(0, -50), vec2(0, 20));
        compView->autoSizing = AutoSizing::HORIZONTAL;
        compView->pixelCorrection = {15, panelHeight};
        AddWidget(compView);
        compView->SetMemberInfo(typeInfo->type_name);
        panelHeight += separation;
        
        CreateMemberView(comp.get(), *typeInfo, 30);
    }
}

void EntityPanel::CreateMemberView(ReflectedObject* _object, const TypeInfo& _typeInfo, float _margin)
{
    for (const auto& member : _typeInfo.members)
    {
        const auto* memberTypeInfo = TypeRegistry::instance().getTypeInfo(Reader::demangleTypeName(member.type_name));
        if (!memberTypeInfo || !HasProperty(member.properties, MemberProperty::Viewable))
        {
            std::shared_ptr<MemberView> memberView = std::make_shared<MemberView>(vec2(0, -50), vec2(0, 20));
            memberView->autoSizing = AutoSizing::HORIZONTAL;
            memberView->pixelCorrection = {_margin, panelHeight};
            AddWidget(memberView);
            memberView->SetMemberInfo(member, _object);
        }
        else
        {
            std::shared_ptr<MemberView> compView = std::make_shared<MemberView>(vec2(0, -50), vec2(0, 20));
            compView->autoSizing = AutoSizing::HORIZONTAL;
            compView->pixelCorrection = {_margin, panelHeight};
            AddWidget(compView);
            compView->SetMemberInfo(Reader::demangleTypeName(member.name));
            panelHeight += separation;
            std::any a = member.getter(_object);
            ReflectedObject* memberObject = any_cast<ReflectedObject*>(a);
            CreateMemberView(memberObject, *memberTypeInfo, _margin + 15);
        }
        panelHeight += separation;
    }
    
}

void EntityPanel::ClearContent()
{
    entity = nullptr;
    children.clear();
    isActive = false;
}


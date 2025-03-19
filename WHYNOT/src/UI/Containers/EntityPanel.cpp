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
    entityView->pixelCorrection = {10, panelHeight};
    AddWidget(entityView);
    entityView->SetMemberInfo(entity->GetName());
    panelHeight += separation;
    
    for (const auto& comp : entity->GetComponents())
    {
        const auto* typeInfo = TypeRegistry::instance().getTypeInfo(Reader::demangleTypeName(typeid(*comp.get()).name()));
        if (!typeInfo)  continue;
        
        std::shared_ptr<MemberView> compView = std::make_shared<MemberView>(vec2(0, -50), vec2(0, 20));
        compView->autoSizing = AutoSizing::HORIZONTAL;
        compView->pixelCorrection = {10, panelHeight};
        AddWidget(compView);
        compView->SetMemberInfo(typeInfo->type_name);
        panelHeight += separation;
        
        CreateMemberView(comp.get(), *typeInfo, 0);
        
        // yCorrection += separation;
        // const auto* typeInfo = TypeRegistry::instance().getTypeInfo(Reader::demangleTypeName(typeid(*comp.get()).name()));
        // if (!typeInfo)  continue;
        //
        //
        // for (const auto& member : typeInfo->members)
        // {
        //     yCorrection += separation;
        //     std::shared_ptr<MemberView> memberView = std::make_shared<MemberView>(vec2(0, -50), vec2(0, 20));
        //     memberView->autoSizing = AutoSizing::HORIZONTAL;
        //     memberView->pixelCorrection = {10, yCorrection};
        //     AddWidget(memberView);
        //     memberView->SetMemberInfo(member, comp.get());
        // }
        // yCorrection += 8;
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
            compView->pixelCorrection = {20, panelHeight};
            AddWidget(compView);
            compView->SetMemberInfo(member.type_name);
            panelHeight += separation;
            
            ReflectedObject* memberObject = static_cast<ReflectedObject*>(member.getter(_object));
            CreateMemberView(memberObject, *memberTypeInfo, _margin + 10);
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


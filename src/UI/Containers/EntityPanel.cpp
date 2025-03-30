#include "EntityPanel.h"

#include "Dropbox.h"
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
    
    std::shared_ptr<MemberView> entityView = std::make_shared<MemberView>(vec2(0, -50), vec2(0, 20), "EntityNameText");
    entityView->autoSizing = AutoSizing::HORIZONTAL;
    entityView->pixelCorrection = {15, 0};
    AddWidget(entityView);
    entityView->SetMemberInfo(entity->GetName());
    
    for (const auto& comp : entity->GetComponents())
    {
        const auto* typeInfo = TypeRegistry::instance().getTypeInfo(Reader::demangleTypeName(typeid(*comp.get()).name()));
        if (!typeInfo)  continue;

        std::shared_ptr<Dropbox> drop = std::make_shared<Dropbox>();
        drop->position = vec2(0, -50);
        drop->autoSizing = AutoSizing::HORIZONTAL;
        drop->pixelCorrection = {15, 0};
        AddWidget(drop);
        drop->OnDroppedDelegate.Bind(&EntityPanel::Reorganize, this);
        
        std::shared_ptr<MemberView> compView = std::make_shared<MemberView>(vec2(0, 0), vec2(0, 20));
        compView->autoSizing = AutoSizing::HORIZONTAL;
        compView->pixelCorrection = {15, 0};
        drop->SetTitleWidget(compView);
        compView->SetMemberInfo(typeInfo->type_name);
        
        CreateMemberView(comp.get(), *typeInfo, 30, drop.get());

        drop->OrganizeWidgets();
    }
    Reorganize();
}

void EntityPanel::CreateMemberView(ReflectedObject* _object, const TypeInfo& _typeInfo, float _margin, Dropbox* _dropbox)
{
    for (const auto& member : _typeInfo.members)
    {
        if (member.name == "name")  continue;
        const auto* memberTypeInfo = TypeRegistry::instance().getTypeInfo(Reader::demangleTypeName(member.type_name));
        if (!memberTypeInfo || !HasProperty(member.properties, MemberProperty::Viewable))
        {
            std::shared_ptr<MemberView> memberView = std::make_shared<MemberView>(vec2(0, 0), vec2(0, 20));
            memberView->autoSizing = AutoSizing::HORIZONTAL;
            _dropbox ? _dropbox->AddDroppingWidget(memberView) : AddWidget(memberView);
            memberView->SetMemberInfo(member, _object);
        }
        else
        {
            std::shared_ptr<Dropbox> drop = std::make_shared<Dropbox>();
            drop->position = vec2(0, 0);
            drop->autoSizing = AutoSizing::HORIZONTAL;
            drop->pixelCorrection = {_margin, 0};

            std::shared_ptr<MemberView> compView = std::make_shared<MemberView>(vec2(0, 0), vec2(0, 20));
            compView->autoSizing = AutoSizing::HORIZONTAL;
            compView->pixelCorrection = {_margin, 0};
            drop ? drop->SetTitleWidget(compView) : AddWidget(compView);

            compView->SetMemberInfo(Reader::demangleTypeName(member.name));
            std::any a = member.getter(_object);
            ReflectedObject* memberObject = any_cast<ReflectedObject*>(a);

            _dropbox->AddDroppingWidget(drop);
            drop->OnDroppedDelegate.Bind(&EntityPanel::Reorganize, this);
            CreateMemberView(memberObject, *memberTypeInfo, _margin + 15, drop.get());
        }
    }
    
}

void EntityPanel::Reorganize()
{
    float h = 10;
    for (const auto& widget : GetChildren())
    {
        widget->pixelCorrection.y = h;
        h += widget->size.y;
        widget->SetPixelPosition();
    }
}


void EntityPanel::ClearContent()
{
    entity = nullptr;
    children.clear();
    isActive = false;
}


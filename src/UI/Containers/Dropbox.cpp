#include "Dropbox.h"

#include <string>
#include <vec2.hpp>
#include <UI/Buttons/DropButton.h>
#include <UI/Text/Text.h>
#include <Utils/Parser.h>

unsigned int Dropbox::counter = 0;

Dropbox::Dropbox(glm::vec2 _pos, glm::vec2 _size, const std::string& _name)
    : Widget(_pos, _size)
{
    name = _name.empty() ? "Dropbox" + std::to_string(++counter) : _name;
}

void Dropbox::Initialize()
{
    Widget::Initialize();

    std::shared_ptr<DropButton> dropButton = std::make_shared<DropButton>();
    dropButton->size = vec2{15, 15};
    dropButton->position = vec2{-50, 0};
    AddWidget(dropButton);
}

void Dropbox::AddDroppingWidget(const shared_ptr<Widget>& _widget)
{
    AddWidget(_widget);
    DroppingWidgets.push_back(_widget.get());
    if (!isDropped)
        _widget->isActive = false;
}

void Dropbox::SetTitleWidget(const shared_ptr<Widget>& _widget)
{
    AddWidget(_widget);
    titleWidget = _widget.get();
}

void Dropbox::CalculateHeight()
{
    size.y = 0;
    for (const auto& widget : GetChildren())
    {
        if (widget->isActive)
            size.y += widget->size.y;
    }
}

void Dropbox::SetAutoName()
{
    if (name.empty())
        name = "Dropbox" + std::to_string(++counter);
}

void Dropbox::OrganizeWidgets()
{
    size.y = titleWidget ? titleWidget->size.y : 0;
    if (!isDropped)
    {
        if (titleWidget) titleWidget->pixelCorrection.y = 0;
        return;
    }

    for (const auto& widget : DroppingWidgets)
    {
        Dropbox* drop = dynamic_cast<Dropbox*>(widget);
        if (drop)
        {
            drop->OrganizeWidgets();
        }
        widget->pixelCorrection.y = size.y;
        size.y += widget->size.y;
    }
}

void Dropbox::Drop()
{
    isDropped = !isDropped;

    for (const auto& widget : DroppingWidgets)
    {
        widget->isActive= isDropped;
    }
    OrganizeWidgets();
    UpdateParentDropboxes();
    parent->SetPixelPosition();
    OnDroppedDelegate.Execute();
}

void Dropbox::UpdateParentDropboxes() const
{
    if (Dropbox* drop = dynamic_cast<Dropbox*>(parent))
    {
        drop->CalculateHeight();
        drop->UpdateParentDropboxes();
    }
}




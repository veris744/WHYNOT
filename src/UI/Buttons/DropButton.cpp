#include "DropButton.h"

#include <UI/Image2D.h>
#include <UI/Containers/Dropbox.h>
#include <Utils/Parser.h>


DropButton::DropButton(const glm::vec2& _pos, const glm::vec2& _size, const std::string& _name)
    : Button(_pos, _size, _name)
{

}

void DropButton::Initialize()
{
    Button::Initialize();

    std::shared_ptr<Image2D> imagePtr = std::make_shared<Image2D>("assets/2dImages/drop.png", glm::vec2(0), glm::vec2(0));
    imagePtr->autoSizing = AutoSizing::ALL;
    imagePtr->hasTransparency = true;
    imagePtr->rotation = 90;

    AddWidget(imagePtr);
    image = imagePtr.get();

    dropbox = static_cast<Dropbox*>(parent);
}


void DropButton::OnClick(glm::vec2 _mousePos)
{
    Button::OnClick(_mousePos);

    if (!dropbox)
        dropbox = static_cast<Dropbox*>(parent);

    dropbox->Drop();

    if (!dropbox->IsDropped())
        image->rotation += 90;
    else
        image->rotation -= 90;

}

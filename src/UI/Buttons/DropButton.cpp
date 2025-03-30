#include "DropButton.h"

#include <UI/Image2D.h>
#include <UI/Containers/Dropbox.h>
#include <Utils/Parser.h>


DropButton::DropButton(const vec2& _pos, const vec2& _size, const string& _name)
    : Button(_pos, _size, _name)
{

}

void DropButton::Initialize()
{
    Button::Initialize();

    std::shared_ptr<Image2D> image = std::make_shared<Image2D>("assets/2dImages/drop.jpg", vec2(0), vec2(0));
    image->autoSizing = AutoSizing::ALL;

    AddWidget(image);

    dropbox = static_cast<Dropbox*>(parent);
}


void DropButton::OnClick(vec2 _mousePos)
{
    Button::OnClick(_mousePos);

    if (!dropbox)
        dropbox = static_cast<Dropbox*>(parent);

    dropbox->Drop();

}

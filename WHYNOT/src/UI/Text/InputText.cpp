#include "InputText.h"

#include "Text.h"
#include "Input/InputManager.h"
#include "UI/Containers/Panel.h"

void InputText::Initialize()
{
    Widget::Initialize();

    if(name.empty())
    {
        SetAutoName();
    }

    size = {300, 100};
    position = {50, 50};
    
    std::shared_ptr<Text> tempText = std::make_shared<Text>();
    textWidget = tempText.get();
    textWidget->align = TextAlign::CENTER;
    textWidget->alignVertical = TextAlignVertical::CENTER;

    std::shared_ptr<Panel> tempPanel = std::make_shared<Panel>();
    panel = tempPanel.get();
    panel->autoSizing = AutoSizing::ALL;
    
    AddWidget(tempText);
    AddWidget(tempPanel);

    InputManager::GetInstance()->OnTypeDelegate.Bind(&InputText::UpdateText, this);
}

void InputText::UpdateText(char ch)
{
    string text = textWidget->text;
    text += ch;
    textWidget->SetText(text);
}

string InputText::ReturnText()
{
    return textWidget->text;
}

void InputText::ClearText()
{
    textWidget->SetText("");
}

void InputText::SetAutoName()
{
    name = "InputText";
}

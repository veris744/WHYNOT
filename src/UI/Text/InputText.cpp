#include "InputText.h"

#include "Text.h"
#include "Input/InputManager.h"
#include "UI/Containers/Panel.h"

void InputText::Initialize()
{
    size = {300, 50};
    position = {0, 0};
    type = WidgetType::Editor;
    
    Widget::Initialize();

    if(name.empty())
    {
        SetAutoName();
    }
    
    std::shared_ptr<Text> tempText = std::make_shared<Text>();
    textWidget = tempText.get();
    textWidget->scale = 0.3f;
    textWidget->align = TextAlign::CENTER;
    textWidget->alignVertical = TextAlignVertical::CENTER;

    std::shared_ptr<Panel> tempPanel = std::make_shared<Panel>();
    panel = tempPanel.get();
    panel->autoSizing = AutoSizing::ALL;
    
    AddWidget(tempText);
    AddWidget(tempPanel);
}

void InputText::UpdateText(char ch)
{
    if (textWidget->text.length() >= maxChars) return;
    
    string text = textWidget->text;
    text += ch;
    textWidget->SetText(text);
}

void InputText::RemoveLastCharacter()
{
    textWidget->SetText(textWidget->text.substr(0, textWidget->text.length() - 1));
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

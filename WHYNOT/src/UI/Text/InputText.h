#pragma once
#include "UI/Widget.h"

class Text;
class Panel;

class InputText : public Widget
{
    Panel* panel = nullptr;
    Text* textWidget = nullptr;

    unsigned int maxChars = 50;
    
public:
    InputText() = default;

    void Initialize() override;

    void UpdateText(char ch);

    string ReturnText();

    void ClearText();
    void SetAutoName() override;
};

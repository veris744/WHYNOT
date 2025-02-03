#pragma once
#include <includes.h>

#include "Input/InputManager.h"

class Widget
{
    InputMode mode = InputMode::UIOnly;
    
protected:
    string name;
    vec2 position;
    vec2 size;
    vec2 finalPos;

    vector<std::shared_ptr<Widget>> children;
    
public:
    Widget() = default;
    Widget(vec2 _pos, vec2 _size, const string& _name = "") : position(_pos), size(_size), finalPos(_pos)
    { }

    virtual ~Widget() = default;

    bool isActive = true;

    void AddWidget(std::shared_ptr<Widget> _widget);
    const vector<std::shared_ptr<Widget>>& GetChildren() const { return children; }
    
    const string& GetName() const { return name; }
    
    InputMode GetInputMode() const { return mode; }
    void SetInputMode(InputMode _mode) { mode = _mode; }
    
    virtual void Render();
    virtual void Clear();
    
};

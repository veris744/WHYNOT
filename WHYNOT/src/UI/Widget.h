#pragma once
#include <includes.h>

#include "Input/InputManager.h"

class Widget : public std::enable_shared_from_this<Widget>
{
protected:
    string name;
    vec2 position;
    vec2 size;

    std::shared_ptr<Widget> parent = nullptr;
    vector<std::shared_ptr<Widget>> children;
    
public:
    Widget() = default;
    Widget(vec2 _pos, vec2 _size, const string& _name = "") : name(_name), position(_pos), size(_size)
    { }

    virtual ~Widget()
    {
        children.clear();
    }

    bool isActive = true;

    void AddWidget(std::shared_ptr<Widget> _widget);
    const vector<std::shared_ptr<Widget>>& GetChildren() const { return children; }
    
    const string& GetName() const { return name; }
    
    virtual void Render();
    virtual void Clear();

    vec2 GetPixelPosition() const;
    
};

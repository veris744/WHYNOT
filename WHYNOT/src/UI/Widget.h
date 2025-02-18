#pragma once
#include <includes.h>
#include "Reflection/Reflection.h"

class Widget : public std::enable_shared_from_this<Widget>
{
protected:

    std::shared_ptr<Widget> parent = nullptr;
    vector<std::shared_ptr<Widget>> children;
    
public:
    Widget(): position(vec2(0,0)), size(vec2(0,0)){}

    Widget(vec2 _pos, vec2 _size, const string& _name = "") : name(_name), position(_pos), size(_size)
    { }

    virtual ~Widget()
    {
        children.clear();
    }
    
    string name;
    vec2 position;
    vec2 size;

    // NOTE: Currently, layers only work with opaque widgets
    unsigned int layer = 0;
    bool hasTransparency = false;

    bool isActive = true;

    void AddWidget(std::shared_ptr<Widget> _widget);
    const vector<std::shared_ptr<Widget>>& GetChildren() const { return children; }
    
    const string& GetName() const { return name; }
    virtual void SetAutoName() = 0;
    
    virtual void Render();
    virtual void Clear();

    vec2 GetPixelPosition() const;
    
};

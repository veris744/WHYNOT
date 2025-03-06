#pragma once
#include <includes.h>
#include "Reflection/Reflection.h"

enum class AutoSizing
{
    NONE, HORIZONTAL, VERTICAL, ALL
};
REGISTER_ENUM(AutoSizing,
    {"NONE", AutoSizing::NONE},
    {"HORIZONTAL", AutoSizing::HORIZONTAL},
    {"VERTICAL", AutoSizing::VERTICAL},
    {"ALL", AutoSizing::ALL},
);


class Widget : public std::enable_shared_from_this<Widget>
{
protected:

    Widget* parent = nullptr;
    vector<std::shared_ptr<Widget>> children;
    
public:
    Widget(): position(vec2(0,0)), size(vec2(0,0)){}

    Widget(vec2 _pos, vec2 _size, const string& _name = "") : name(_name), position(_pos), size(_size)
    { }

    virtual ~Widget()
    {
        //Logger::Log(LogLevel::Info, "Widget Destructor " + name);
        children.clear();
    }
    virtual void Initialize() {}
    
    string name;
    vec2 position;
    vec2 size;
    AutoSizing autoSizing = AutoSizing::NONE;
    
    // top, right, bottom, left (TEMP IMPLEMENTATION, ONLY WITHOUT AUTOSIZING)
    vec2 pixelCorrection = {0,0};

    // NOTE: Currently, layers only work with opaque widgets
    unsigned int layer = 0;
    bool hasTransparency = false;

    bool isActive = true;

    void AddWidget(const std::shared_ptr<Widget>& _widget);
    const vector<std::shared_ptr<Widget>>& GetChildren() const { return children; }
    void ClearChildren() {  children.clear(); }
    
    const string& GetName() const { return name; }
    virtual void SetAutoName() = 0;
    
    virtual void Render();
    virtual void Clear();

    void Destroy();

    vec2 GetPixelPosition() const;
    vec2 GetAutoSize() const;
    
};

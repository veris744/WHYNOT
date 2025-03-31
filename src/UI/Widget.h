#pragma once

#include "Input/InputManager.h"
#include "Managers/Helper.h"
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


class Widget : public std::enable_shared_from_this<Widget>, public ReflectedObject
{
protected:

    Widget* parent = nullptr;
    vector<std::shared_ptr<Widget>> children;
    vec2 pixelPosition = {0, 0};
    
public:
    Widget(): position(vec2(0,0)), size(vec2(0,0)){}

    Widget(vec2 _pos, vec2 _size, const string& _name = "") : name(_name), position(_pos), size(_size)
    { }

    virtual ~Widget()
    {
        //Logger::Log(LogLevel::Info, "Widget Destructor " + name);
        children.clear();
        Helper::OnWindowResizeDelegate.Remove(this);
    }
    virtual void Initialize()
    {
        size = GetAutoSize();
        SetPixelPosition();
        Helper::OnWindowResizeDelegate.Add(&Widget::OnWindowResize, this);
    }

    string name;
    vec2 position = vec2(0);
    vec2 size = vec2(0);
    AutoSizing autoSizing = AutoSizing::NONE;
    
    // top, right, bottom, left (TEMP IMPLEMENTATION, ONLY WITHOUT AUTOSIZING)
    vec2 pixelCorrection = {0,0};

    // NOTE: Currently, layers only work with opaque widgets
    unsigned int layer = 0;
    bool hasTransparency = false;

    bool isActive = true;
    bool isBlocking = false;

    void AddWidget(const std::shared_ptr<Widget>& _widget);
    const vector<std::shared_ptr<Widget>>& GetChildren() const { return children; }
    void ClearChildren() {  children.clear(); }
    
    const string& GetName() const { return name; }
    virtual void SetAutoName() = 0;
    void SetActiveWithChildren(bool _active);
    bool AreParentsActive();
    Widget* GetParent() const { return parent; }

    bool IsClicking(const vec2& _mousePos) const;
    virtual void OnClick(vec2 _mousePos) {}
    
    virtual void Render();
    virtual void Clear();

    void Destroy();
    void RemoveChild(const std::shared_ptr<Widget>& _widget);

    vec2 GetPixelPosition() const;
    void SetPixelPosition();
    vec2 GetAutoSize() const;

    void OnWindowResize();

};

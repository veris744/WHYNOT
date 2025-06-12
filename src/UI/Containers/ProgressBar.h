#pragma once
#include "UI/Widget.h"

class Panel;

class ProgressBar : public Widget
{
    static unsigned int counter;

    float value = 0;

    Panel* frontPanel = nullptr;

protected:
    void Initialize() override;

public:
    ProgressBar(vec2 _pos, vec2 _size, const string& _name = "");
    ProgressBar() = default;
    void Render() override;
    void Clear() override;
    void SetAutoName() override;

    void UpdateValue(float _value);
    float GetValue() const { return value * 100; };
};

#pragma once
#include <string>

#include "UI/Widget.h"

class Text;

class MemberView : public Widget
{
    static unsigned int counter;
    void Initialize() override;
    vec3 color = vec3(1);
    float scale = 0.5f;

    Text * text = nullptr;
    
public:
    MemberView(vec2 _pos, vec2 _size, const string& _name = "");
    MemberView() = default;
    void SetAutoName() override;
    void SetMemberInfo(const MemberInfo& info, const string& value = "");
    void SetMemberInfo(const string& value);
};

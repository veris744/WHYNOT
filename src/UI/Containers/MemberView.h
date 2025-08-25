#pragma once
#include <string>

#include "UI/Widget.h"
#include "UI/Buttons/Button.h"

class Component;
class Text;

class MemberView : public Widget
{
    static unsigned int counter;
    void Initialize() override;
    glm::vec3 color = glm::vec3(1);
    float scale = 0.3f;

    ReflectedObject * object = nullptr;
    const MemberInfo* memberInfo = nullptr;
    Text * textWidget = nullptr;
    Button * button = nullptr;

    bool isMember = true;
    
public:
    MemberView(glm::vec2 _pos, glm::vec2 _size, const std::string& _name = "");
    MemberView() = default;
    void SetAutoName() override;
    void SetMemberInfo(const MemberInfo& _info, ReflectedObject* _object);
    void SetMemberInfo(const std::string& value) const;
    void UpdateMember(const std::string& value);
    void SetUpdateButton();
    void SetRedirectButton(Entity * _entity);

    void SetIsMember(bool _isMember) { isMember = _isMember; };
    bool IsMember() const { return isMember; }
};

#pragma once

#include "Component.h"
#include "Reflection/Reflection.h"

enum class LightSourceType
{
    DIRECTIONAL, POINT, SPOT, AMBIENT
};
REGISTER_ENUM(LightSourceType, 
    {"DIRECTIONAL", LightSourceType::DIRECTIONAL},
    {"POINT", LightSourceType::POINT},
    {"SPOT", LightSourceType::SPOT},
    {"AMBIENT", LightSourceType::AMBIENT}
);

struct LightData
{
    vec4  position = vec4(0.f, 0.f,0.f,1);
    vec4  color = vec4(0, 0,0,1);
    vec3  direction = vec3(0);

    float specular    = 0.f;
    float diffuse     = 0.f;
    float ambient     = 0.f;

    float constantAtt = 0.f;
    float linearAtt = 0.f;
    float quadAtt = 0.f;

    float angle = 0.f;
    float outerAngle = 0.f;

    LightSourceType type = LightSourceType::DIRECTIONAL;
};
REGISTER_CLASS(LightData, {
    REGISTER_MEMBER(LightData, position, MemberProperty::Viewable),
    REGISTER_MEMBER(LightData, color, MemberProperty::Viewable),
    REGISTER_MEMBER(LightData, direction, MemberProperty::Viewable),
    REGISTER_MEMBER(LightData, specular, MemberProperty::Viewable),
    REGISTER_MEMBER(LightData, diffuse, MemberProperty::Viewable),
    REGISTER_MEMBER(LightData, ambient, MemberProperty::Viewable),
    REGISTER_MEMBER(LightData, constantAtt, MemberProperty::Viewable),
    REGISTER_MEMBER(LightData, linearAtt, MemberProperty::Viewable),
    REGISTER_MEMBER(LightData, quadAtt, MemberProperty::Viewable),
    REGISTER_MEMBER(LightData, angle, MemberProperty::Viewable),
    REGISTER_MEMBER(LightData, outerAngle, MemberProperty::Viewable),
    REGISTER_MEMBER(LightData, type, MemberProperty::NonViewable)
});

class LightSource : public Component
{
    
public:
    LightSource()
    {
        name = "LIGHT";
        lightData = LightData();
    }
    ~LightSource() override = default;

    void Update(float deltaTime) override
    {
        
    }
    LightData lightData;    
};
REGISTER_CLASS(LightSource, {
    REGISTER_MEMBER(LightSource, lightData, MemberProperty::NonViewable)
});
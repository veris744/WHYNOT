#pragma once
#include <includes.h>


enum class LightSourceType
{
    DIRECTIONAL, POINT, SPOT, AMBIENT
};

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

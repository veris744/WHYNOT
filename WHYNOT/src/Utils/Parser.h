#pragma once
#include "includes.h"

namespace Parser
{
    inline string Parse(unsigned int data)
    {
        return std::to_string(data);
    }
    
    inline string Parse(int data)
    {
        return std::to_string(data);
    }
    
    inline string Parse(float data)
    {
        return std::to_string(data);
    }
    
    inline string Parse(const vec2& data)
    {
        return "(" + std::to_string(data.x) + ", " + std::to_string(data.y) + ")";
    }
    
    inline string Parse(const vec3& data)
    {
        return "(" + std::to_string(data.x) + ", " + std::to_string(data.y) + ", " + std::to_string(data.z) + ")";
    }
    
};

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
    inline string Parse(const mat4& data)
    {
        string res = "((" + std::to_string(data[0][0]) + ", " + std::to_string(data[0][1]) + ", " + std::to_string(data[0][2]) + ", " + std::to_string(data[0][3]) + "),\n"
            + "(" + std::to_string(data[1][0]) + ", " + std::to_string(data[1][1]) + ", " + std::to_string(data[1][2]) + ", " + std::to_string(data[1][3]) + "),\n"
            + "(" + std::to_string(data[2][0]) + ", " + std::to_string(data[2][1]) + ", " + std::to_string(data[2][2]) + ", " + std::to_string(data[2][3]) + "),\n"
            + "(" + std::to_string(data[3][0]) + ", " + std::to_string(data[3][1]) + ", " + std::to_string(data[3][2]) + ", " + std::to_string(data[3][3]) + "))";
        return res;
    }
    inline string Parse(const glm::quat& data)
    {
        return "(" + std::to_string(data.w) + ", " + std::to_string(data.x) + ", " + std::to_string(data.y) + ", " + std::to_string(data.z) + ")";
    }
    
};

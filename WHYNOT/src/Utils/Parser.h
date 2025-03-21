#pragma once
#include <iomanip>
#include <sstream>
#include <glm/detail/type_quat.hpp>

#include "glm/glm.hpp"
#include "Reader/EnumRegistry.h"

//#include "Reader/EnumRegistry.h"

using namespace glm;
using namespace std;

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
    
    inline string Parse(float data, unsigned int precision = 2)
    {
        std::ostringstream stream;
        stream << std::fixed << std::setprecision(precision) << data;
        return stream.str();
    }
    
    inline string Parse(const vec2& data, unsigned int precision = 2)
    {
        std::ostringstream stream;
        stream << std::fixed << std::setprecision(precision)
               << "(" << data.x << ", " << data.y << ")";
        return stream.str();
    }
    
    inline string Parse(const vec3& data, unsigned int precision = 2)
    {
        std::ostringstream stream;
        stream << std::fixed << std::setprecision(precision)
               << "(" << data.x << ", " << data.y << ", " << data.z << ")";
        return stream.str();
    }
    inline string Parse(const vec4& data, unsigned int precision = 2)
    {
        std::ostringstream stream;
        stream << std::fixed << std::setprecision(precision)
               << "(" << data.x << ", " << data.y << ", " << data.z << ", " << data.w << ")";
        return stream.str();
    }
    inline string Parse(const mat4& data)
    {
        string res = "((" + std::to_string(data[0][0]) + ", " + std::to_string(data[0][1]) + ", " + std::to_string(data[0][2]) + ", " + std::to_string(data[0][3]) + "),\n"
            + "(" + std::to_string(data[1][0]) + ", " + std::to_string(data[1][1]) + ", " + std::to_string(data[1][2]) + ", " + std::to_string(data[1][3]) + "),\n"
            + "(" + std::to_string(data[2][0]) + ", " + std::to_string(data[2][1]) + ", " + std::to_string(data[2][2]) + ", " + std::to_string(data[2][3]) + "),\n"
            + "(" + std::to_string(data[3][0]) + ", " + std::to_string(data[3][1]) + ", " + std::to_string(data[3][2]) + ", " + std::to_string(data[3][3]) + "))";
        return res;
    }
    inline string Parse(const quat& data)
    {
        return "(" + std::to_string(data.w) + ", " + std::to_string(data.x) + ", " + std::to_string(data.y) + ", " + std::to_string(data.z) + ")";
    }
    inline string Parse(const string& data)
    {
        return data;
    }

    inline std::string ParseValue(void* value, const string& type) {
        if (type == "int") {
            return Parse(*static_cast<int*>(value));
        }
        if (type == "float") {
            return Parse(*static_cast<float*>(value));
        }
        if (type == "bool") {
            return *static_cast<bool*>(value) ? "true" : "false";
        }
        if (type.find("string") != std::string::npos) {
            return *static_cast<std::string*>(value);
        }
        if (type == "struct glm::vec<2,float,0>") {
            return Parse(*static_cast<vec2*>(value));
        }
        if (type == "struct glm::vec<3,float,0>") {
            return Parse(*static_cast<vec3*>(value));
        }
        if (type == "struct glm::vec<4,float,0>") {
            return Parse(*static_cast<vec4*>(value));
        }
        if (type.find("enum") != std::string::npos)
        {
            return EnumRegistry::instance().getEnumStringFromValue(type, *static_cast<int*>(value));
        }
        return "Unknown";
    }
    
    
};

#pragma once
#include <any>
#include <iomanip>
#include <sstream>
#include <glm/detail/type_quat.hpp>

#include "glm/glm.hpp"
#include "Reader/EnumRegistry.h"

//#include "Reader/EnumRegistry.h"

namespace Parser
{
    inline std::string Parse(unsigned int data)
    {
        return std::to_string(data);
    }
    
    inline std::string Parse(int data)
    {
        return std::to_string(data);
    }
    
    inline std::string Parse(float data, unsigned int precision = 2)
    {
        std::ostringstream stream;
        stream << std::fixed << std::setprecision(precision) << data;
        return stream.str();
    }
    
    inline std::string Parse(const glm::vec2& data, unsigned int precision = 2)
    {
        std::ostringstream stream;
        stream << std::fixed << std::setprecision(precision)
               << "(" << data.x << ", " << data.y << ")";
        return stream.str();
    }
    
    inline std::string Parse(const glm::vec3& data, unsigned int precision = 2)
    {
        std::ostringstream stream;
        stream << std::fixed << std::setprecision(precision)
               << "(" << data.x << ", " << data.y << ", " << data.z << ")";
        return stream.str();
    }
    inline std::string Parse(const glm::vec4& data, unsigned int precision = 2)
    {
        std::ostringstream stream;
        stream << std::fixed << std::setprecision(precision)
               << "(" << data.x << ", " << data.y << ", " << data.z << ", " << data.w << ")";
        return stream.str();
    }
    inline std::string Parse(const glm::mat4& data)
    {
        std::string res = "((" + std::to_string(data[0][0]) + ", " + std::to_string(data[0][1]) + ", " + std::to_string(data[0][2]) + ", " + std::to_string(data[0][3]) + "),\n"
            + "(" + std::to_string(data[1][0]) + ", " + std::to_string(data[1][1]) + ", " + std::to_string(data[1][2]) + ", " + std::to_string(data[1][3]) + "),\n"
            + "(" + std::to_string(data[2][0]) + ", " + std::to_string(data[2][1]) + ", " + std::to_string(data[2][2]) + ", " + std::to_string(data[2][3]) + "),\n"
            + "(" + std::to_string(data[3][0]) + ", " + std::to_string(data[3][1]) + ", " + std::to_string(data[3][2]) + ", " + std::to_string(data[3][3]) + "))";
        return res;
    }
    inline std::string Parse(const glm::quat& data)
    {
        return "(" + std::to_string(data.w) + ", " + std::to_string(data.x) + ", " + std::to_string(data.y) + ", " + std::to_string(data.z) + ")";
    }
    inline std::string Parse(const std::string& data)
    {
        return data;
    }

    inline std::string ParseValue(const std::any& value, const std::string& type) {
        try {
            std::string _type = value.type().name();
            if (value.type() == typeid(std::reference_wrapper<int>)) {
                return Parse(std::any_cast<std::reference_wrapper<int>>(value).get());
            }
            if (value.type() == typeid(std::reference_wrapper<float>)) {
                return Parse(std::any_cast<std::reference_wrapper<float>>(value).get());
            }
            if (value.type() == typeid(std::reference_wrapper<bool>)) {
                return std::any_cast<std::reference_wrapper<bool>>(value).get() ? "true" : "false";
            }
            if (value.type() == typeid(std::reference_wrapper<std::string>)) {
                return std::any_cast<std::reference_wrapper<std::string>>(value).get();
            }
            if (value.type() == typeid(std::reference_wrapper<glm::vec2>)) {
                return Parse(std::any_cast<std::reference_wrapper<glm::vec2>>(value).get());
            }
            if (value.type() == typeid(std::reference_wrapper<glm::vec3>)) {
                return Parse(std::any_cast<std::reference_wrapper<glm::vec3>>(value).get());
            }
            if (value.type() == typeid(std::reference_wrapper<glm::vec4>)) {
                return Parse(std::any_cast<std::reference_wrapper<glm::vec4>>(value).get());
            }
            if (_type.find('enum') != std::string::npos) {
                return EnumRegistry::instance().getEnumFromAny(type, value);
            }
        } catch (const std::bad_any_cast&) {
            return "Invalid type conversion";
        }

        return "Unknown";
    }
    
    
};

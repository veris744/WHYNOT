#pragma once
#include "glm/glm.hpp"

#ifndef YAML_CPP_STATIC_DEFINE
#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>
#endif

#include <Utils/Debugger.h>

#include "EnumRegistry.h"
#include "Utils/Logger.h"

using namespace glm;

namespace Reader
{
    inline string demangleTypeName(const string& mangledName)
    {
        string result = mangledName;
        const string classPrefix = "class ";
        const string structPrefix = "struct ";
        if (result.find(classPrefix) == 0) {
            result = result.substr(classPrefix.size());
        } else if (result.find(structPrefix) == 0) {
            result = result.substr(structPrefix.size());
        }
        return result;
    }
    
    vec3 ReadVec3(const YAML::Node& node, const string& name, const vec3& defaultValue = vec3(0));
    vec4 ReadVec4(const YAML::Node& node, const string& name, const vec4& defaultValue = vec4(0));
    vec2 ReadVec2(const YAML::Node& node, const string& name, const vec2& defaultValue = vec2(0));
    string ReadString(const YAML::Node& node, const string& name, const string& defaultValue = string(""));
    int ReadInt(const YAML::Node& node, const string& name, int defaultValue = 0);
    unsigned int ReadUInt(const YAML::Node& node, const string& name, unsigned int defaultValue = 0);
    float ReadFloat(const YAML::Node& node, const string& name, float defaultValue = 0.0f);
    bool ReadBool(const YAML::Node& node, const string& name, bool defaultValue = false);
    vector<string> ReadStringVector(const YAML::Node& node, const std::string& name);

    inline YAML::Node ConvertMemberToYaml(string _memberName, string _value)
    {
        try
        {
            string res;
            bool isVector = _value.find(',') != std::string::npos;
            YAML::Node node;
            if (isVector)
            {
                std::vector<std::string> values;
                std::stringstream ss(_value);
                std::string item;

                while (std::getline(ss, item, ','))
                {
                    values.push_back(item);
                }
                for (const auto& v : values)
                {
                    node[_memberName].push_back(v);
                }
            }
            else
            {
                node[_memberName] = _value;
            }
            return node;
        }
        catch (...)
        {
            Logger::Log(LogLevel::Error, "Incorrect format for conversion");
        }
    }
    
    template<typename T>
    T ReadValue(const YAML::Node& node, const string& name)
    {
        if constexpr (is_enum_v<T>) {
            string valueStr = node[name].as<string>();
            return EnumRegistry::instance().fromString<T>(valueStr);
        }
        else if constexpr (is_same_v<T, decltype(vec3{})>) {
            return ReadVec3(node, name);
        }
        if constexpr (is_same_v<T, decltype(vec4{})>) {
            return ReadVec4(node, name);
        }
        else if constexpr (is_same_v<T, decltype(vec2{})>) {
            return ReadVec2(node, name);
        }
        else if constexpr (is_same_v<T, vector<string>>) {
            return ReadStringVector(node, name);
        }
        else if constexpr (is_same_v<T, string>) {
            return ReadString(node, name);
        }
        else if constexpr (is_same_v<T, int>) {
            return ReadInt(node, name);
        }
        else if constexpr (is_same_v<T, unsigned int>) {
            return ReadUInt(node, name);
        }
        else if constexpr (is_same_v<T, float>) {
            return ReadFloat(node, name);
        }
        else if constexpr (is_same_v<T, bool>) {
            return ReadBool(node, name);
        }
        Logger::Log(LogLevel::Error, "Unsupported type");
    }

    template<typename T>
    constexpr bool IsGLMType() {
        return is_same_v<T, decltype(vec2{})> || is_same_v<T, decltype(vec3{})> ||
               is_same_v<T, decltype(vec4{})> || is_same_v<T, decltype(mat4{})>;
    }
}

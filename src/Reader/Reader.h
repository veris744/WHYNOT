#pragma once
#include "glm/glm.hpp"

#ifndef YAML_CPP_STATIC_DEFINE
#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>
#endif

#include <Utils/Debugger.h>

#include "EnumRegistry.h"
#include "Utils/Logger.h"


namespace Reader
{
    inline std::string demangleTypeName(const std::string& mangledName)
    {
        std::string result = mangledName;
        const std::string classPrefix = "class ";
        const std::string structPrefix = "struct ";
        if (result.find(classPrefix) == 0) {
            result = result.substr(classPrefix.size());
        } else if (result.find(structPrefix) == 0) {
            result = result.substr(structPrefix.size());
        }
        return result;
    }
    
    glm::vec3 ReadVec3(const YAML::Node& node, const std::string& name, const glm::vec3& defaultValue = glm::vec3(0));
    glm::vec4 ReadVec4(const YAML::Node& node, const std::string& name, const glm::vec4& defaultValue = glm::vec4(0));
    glm::vec2 ReadVec2(const YAML::Node& node, const std::string& name, const glm::vec2& defaultValue = glm::vec2(0));
    std::string ReadString(const YAML::Node& node, const std::string& name, const std::string& defaultValue = std::string(""));
    int ReadInt(const YAML::Node& node, const std::string& name, int defaultValue = 0);
    unsigned int ReadUInt(const YAML::Node& node, const std::string& name, unsigned int defaultValue = 0);
    float ReadFloat(const YAML::Node& node, const std::string& name, float defaultValue = 0.0f);
    bool ReadBool(const YAML::Node& node, const std::string& name, bool defaultValue = false);
    std::vector<std::string> ReadStringVector(const YAML::Node& node, const std::string& name);

    inline YAML::Node ConvertMemberToYaml(std::string _memberName, std::string _value)
    {
        try
        {
            std::string res;
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
    T ReadValue(const YAML::Node& node, const std::string& name)
    {
        if constexpr (std::is_enum_v<T>) {
            std::string valueStr = node[name].as<std::string>();
            return EnumRegistry::instance().fromString<T>(valueStr);
        }
        else if constexpr (std::is_same_v<T, decltype(glm::vec3{})>) {
            return ReadVec3(node, name);
        }
        if constexpr (std::is_same_v<T, decltype(glm::vec4{})>) {
            return ReadVec4(node, name);
        }
        else if constexpr (std::is_same_v<T, decltype(glm::vec2{})>) {
            return ReadVec2(node, name);
        }
        else if constexpr (std::is_same_v<T, std::vector<std::string>>) {
            return ReadStringVector(node, name);
        }
        else if constexpr (std::is_same_v<T, std::string>) {
            return ReadString(node, name);
        }
        else if constexpr (std::is_same_v<T, int>) {
            return ReadInt(node, name);
        }
        else if constexpr (std::is_same_v<T, unsigned int>) {
            return ReadUInt(node, name);
        }
        else if constexpr (std::is_same_v<T, float>) {
            return ReadFloat(node, name);
        }
        else if constexpr (std::is_same_v<T, bool>) {
            return ReadBool(node, name);
        }
        Logger::Log(LogLevel::Error, "Unsupported type");
    }

    template<typename T>
    constexpr bool IsGLMType() {
        return std::is_same_v<T, decltype(glm::vec2{})> || std::is_same_v<T, decltype(glm::vec3{})> ||
               std::is_same_v<T, decltype(glm::vec4{})> || std::is_same_v<T, decltype(glm::mat4{})>;
    }
}

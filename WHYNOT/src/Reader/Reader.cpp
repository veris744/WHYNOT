#include "Reader.h"

#include "Utils/Parser.h"

namespace Reader
{
    vec3 ReadVec3(const YAML::Node& node, const string& name, const vec3& defaultValue)
    {
        return node[name] ?
            vec3(node[name][0].as<float>(),
             node[name][1].as<float>(),
             node[name][2].as<float>()) : defaultValue;
    }

    vec4 ReadVec4(const YAML::Node& node, const string& name, const vec4& defaultValue)
    {
        return node[name] ?
            vec4(node[name][0].as<float>(),
             node[name][1].as<float>(),
             node[name][2].as<float>(),
             node[name][3] ? node[name][3].as<float>() : 1) : defaultValue;
    }

    vec2 ReadVec2(const YAML::Node& node, const string& name, const vec2& defaultValue)
    {
        return node[name] ?
            vec2(node[name][0].as<float>(),
             node[name][1].as<float>()) : defaultValue;
    }

    string ReadString(const YAML::Node& node, const string& name, const string& defaultValue)
    {
        return node[name] ? node[name].as<string>() : defaultValue;
    }

    int ReadInt(const YAML::Node& node, const string& name, int defaultValue)
    {
        return node[name] ? node[name].as<int>() : defaultValue;
    }

    unsigned int ReadUInt(const YAML::Node& node, const string& name, unsigned int defaultValue)
    {
        return node[name] ? node[name].as<unsigned int>() : defaultValue;
    }

    float ReadFloat(const YAML::Node& node, const string& name, float defaultValue)
    {
        if (node[name] && node[name].IsDefined()) {
            try {
                return node[name].as<float>();
            } catch (const std::exception& e) {
                // Handle the exception (optional)
                std::cerr << "Error converting YAML node to float: " << e.what() << std::endl;
            }
        }
        return defaultValue;
    }

    bool ReadBool(const YAML::Node& node, const string& name, bool defaultValue)
    {
        return node[name] ? node[name].as<bool>() : defaultValue;
    }

    vector<string> ReadStringVector(const YAML::Node& node, const std::string& name)
    {
        std::vector<std::string> result;
        if (node[name] && node[name].IsSequence()) {
            for (const auto& item : node[name]) {
                result.push_back(item.as<std::string>());
            }
        }
        return result;
    }

}

#include "Reader.h"

namespace Reader
{
    vec3 ReadVec3(const YAML::Node& node, const string& name, const vec3& defaultValue)
    {
        return node[name] ?
            vec3(node[name][0].as<float>(),
             node[name][1].as<float>(),
             node[name][2].as<float>()) : defaultValue;
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
        return node[name] ? node[name].as<float>() : defaultValue;
    }

    bool ReadBool(const YAML::Node& node, const string& name, bool defaultValue)
    {
        return node[name] ? node[name].as<bool>() : defaultValue;
    }
}

#pragma once
#include <includes.h>

namespace Reader
{
    vec3 ReadVec3(const YAML::Node& node, const string& name, const vec3& defaultValue = vec3(0));
    vec2 ReadVec2(const YAML::Node& node, const string& name, const vec2& defaultValue = vec2(0));
    string ReadString(const YAML::Node& node, const string& name, const string& defaultValue = string(""));
    int ReadInt(const YAML::Node& node, const string& name, int defaultValue = 0);
    unsigned int ReadUInt(const YAML::Node& node, const string& name, unsigned int defaultValue = 0);
    float ReadFloat(const YAML::Node& node, const string& name, float defaultValue = 0.0f);
    bool ReadBool(const YAML::Node& node, const string& name, bool defaultValue = false);
};

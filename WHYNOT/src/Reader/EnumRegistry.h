#pragma once
#include <stdexcept>
#include <string>
#include <unordered_map>

using namespace std;

struct EnumInfo {
    unordered_map<string, int> nameToValue;
    unordered_map<int, string> valueToName;
};

class EnumRegistry
{
    unordered_map<string, EnumInfo> enums;

public:
    static EnumRegistry& instance() {
        static EnumRegistry instance;
        return instance;
    }
    
    static string demangleEnumType(const string& mangledName)
    {
        string result = mangledName;
        const string enumPrefix = "enum ";
        if (result.find(enumPrefix) == 0) {
            result = result.substr(enumPrefix.size());
        }
        return result;
    }

    template<typename EnumType>
    void registerEnum(const initializer_list<pair<string, EnumType>>& entries) {
        const string enumName = typeid(EnumType).name();
        EnumInfo& info = enums.emplace(enumName, EnumInfo()).first->second;
        
        for (const auto& entry : entries) {
             info.nameToValue[entry.first] = static_cast<int>(entry.second);
             info.valueToName[static_cast<int>(entry.second)] = entry.first;
        }
    }

    template<typename EnumType>
    EnumType fromString(const string& name) const {
        const string enumName = typeid(EnumType).name();
        auto it = enums.find(enumName);
        if (it != enums.end()) {
            const auto& info = it->second;
            auto valIt = info.nameToValue.find(name);
            if (valIt != info.nameToValue.end()) {
                return static_cast<EnumType>(valIt->second);
            }
        }
        throw invalid_argument("Invalid enum string: " + name);
    }

    template<typename EnumType>
    string toString(EnumType value) const {
        const string enumName = typeid(EnumType).name();
        auto it = enums.find(enumName);
        if (it != enums.end()) {
            const auto& info = it->second;
            auto valIt = info.valueToName.find(static_cast<int>(value));
            if (valIt != info.valueToName.end()) {
                return valIt->second;
            }
        }
        return "UnknownEnumValue";
    }

    string getEnumStringFromValue(const string& enumType, int enumValue) const {
        auto it = enums.find(enumType);
        if (it != enums.end()) {
            const auto& info = it->second;
            auto valIt = info.valueToName.find(enumValue);
            if (valIt != info.valueToName.end()) {
                return valIt->second;  // Return the enum string name
            }
        }
        return "UnknownEnumValue";  // Return a default value if not found
    }
};

#define REGISTER_ENUM(EnumType, ...) \
    namespace { \
        struct EnumType##Register { \
            EnumType##Register() { \
                EnumRegistry::instance().registerEnum<EnumType>({__VA_ARGS__}); \
            } \
        }; \
        static EnumType##Register global_##EnumType##Register; \
    }

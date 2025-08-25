#pragma once
#include <any>
#include <functional>
#include <stdexcept>
#include <string>
#include <unordered_map>


struct EnumInfo {
    std::unordered_map<std::string, int> nameToValue;
    std::unordered_map<int, std::string> valueToName;
};

class EnumRegistry
{
public:
    using EnumCaster = std::function<int(const std::any&)>;

    static EnumRegistry& instance() {
        static EnumRegistry instance;
        return instance;
    }

    static std::string demangleEnumType(const std::string& mangledName)
    {
        std::string result = mangledName;
        const std::string enumPrefix = "enum ";
        if (result.find(enumPrefix) == 0) {
            result = result.substr(enumPrefix.size());
        }
        return result;
    }

private:
    std::unordered_map<std::string, EnumInfo> enums;
    std::unordered_map<std::string, EnumCaster> enumCasters;

public:

    void registerEnumCaster(const std::string& enumName, EnumCaster caster) {
        enumCasters[enumName] = caster;
    }

    int castEnumToInt(const std::string& enumName, const std::any& value) const {
        auto it = enumCasters.find(enumName);
        if (it != enumCasters.end()) {
            return it->second(value); // Call the registered caster function
        }
        throw std::runtime_error("Unknown enum type: " + enumName);
    }

    template<typename EnumType>
    void registerEnum(const std::initializer_list<std::pair<std::string, EnumType>>& entries) {
        const std::string enumName = typeid(EnumType).name();
        EnumInfo& info = enums.emplace(enumName, EnumInfo()).first->second;
        
        for (const auto& entry : entries) {
             info.nameToValue[entry.first] = static_cast<int>(entry.second);
             info.valueToName[static_cast<int>(entry.second)] = entry.first;
        }

        enumCasters[enumName] = [](const std::any& value) -> int {
            return static_cast<int>(std::any_cast<std::reference_wrapper<EnumType>>(value).get());
        };
    }

    template<typename EnumType>
    EnumType fromString(const std::string& name) const {
        const std::string enumName = typeid(EnumType).name();
        auto it = enums.find(enumName);
        if (it != enums.end()) {
            const auto& info = it->second;
            auto valIt = info.nameToValue.find(name);
            if (valIt != info.nameToValue.end()) {
                return static_cast<EnumType>(valIt->second);
            }
        }
        throw std::invalid_argument("Invalid enum std::string: " + name);
    }

    template<typename EnumType>
    std::string toString(EnumType value) const {
        const std::string enumName = typeid(EnumType).name();
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

    std::string getEnumStringFromValue(const std::string& enumType, int enumValue) const {
        auto it = enums.find(enumType);
        if (it != enums.end()) {
            const auto& info = it->second;
            auto valIt = info.valueToName.find(enumValue);
            if (valIt != info.valueToName.end()) {
                return valIt->second;  // Return the enum std::string name
            }
        }
        return "UnknownEnumValue";  // Return a default value if not found
    }

    std::string getEnumFromAny(const std::string& enumType, const std::any& value) const
    {
        int intValue = castEnumToInt(enumType, value);
        return getEnumStringFromValue(enumType, intValue);
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

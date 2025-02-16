#pragma once
#include <functional>
#include <string>
#include <unordered_map>
#include <Reader/Reader.h>

using namespace std;
using FactoryFunction = std::function<void*()>;

struct MemberInfo
{
    string name;
    string type_name;
    function<void(void*, const YAML::Node&)> setter;
};

struct TypeInfo {
    std::string type_name;
    std::vector<MemberInfo> members;
};


class TypeRegistry
{
    unordered_map<std::string, TypeInfo> types;
    unordered_map<std::string, std::function<void*()>> factories;
    
public:
    TypeRegistry() = default;
    ~TypeRegistry() = default;
    TypeRegistry(const TypeRegistry&) = delete;
    TypeRegistry& operator=(const TypeRegistry&) = delete;
    
    static TypeRegistry& instance() {
        static TypeRegistry registry;
        return registry;
    }
    
    void registerType(const std::string& typeName, const std::vector<MemberInfo>& members) {
        types[typeName] = {typeName, members};
    }
    
    const TypeInfo* getTypeInfo(const string& typeName) const {
        auto it = types.find(typeName);
        return it != types.end() ? &it->second : nullptr;
    }
    
    void registerFactory(const string& typeName, FactoryFunction factory) {
        factories[typeName] = factory;
    }

    void* createInstance(const string& typeName) const {
        auto it = factories.find(typeName);
        return it != factories.end() ? it->second() : nullptr;
    }
};


namespace Reflection
{
    template <typename T>
    void deserialize(const YAML::Node& node, T& obj) {
        string name = Reader::demangleTypeName(typeid(T).name());
        const auto* typeInfo = TypeRegistry::instance().getTypeInfo(Reader::demangleTypeName(typeid(T).name()));
        if (typeInfo) {
            for (const auto& member : typeInfo->members) {
                member.setter(&obj, node);
            }
        } else {
            throw std::runtime_error("Type not registered: " + std::string(typeid(T).name()));
        }
    }
    
    template <typename T>
    void deserialize(const YAML::Node& node, std::shared_ptr<T>& ptr) {
        if (!ptr) {
            ptr = std::make_shared<T>();
        }
        deserialize(node, *ptr);
    }

    template <typename T>
    void deserialize(const YAML::Node& node, std::unique_ptr<T>& ptr) {
        if (!ptr) {
            ptr = std::make_unique<T>();
        }
        deserialize(node, *ptr);
    }
}


#define REGISTER_CLASS(type, ...) \
    namespace { \
        struct type##Register { \
            type##Register() { \
                TypeRegistry::instance().registerType(#type, std::vector<MemberInfo>{__VA_ARGS__}); \
                TypeRegistry::instance().registerFactory(#type, []() -> void* { return new type(); }); \
            } \
        }; \
        static type##Register global_##type##Register; \
    }

#define REGISTER_MEMBER(type, member) \
    MemberInfo{#member, #type, [](void* obj, const YAML::Node& node) { \
        auto& instance = *static_cast<type*>(obj); \
        using MemberT = std::decay_t<decltype(instance.member)>; \
        if constexpr (std::is_class_v<MemberT> && !Reader::IsGLMType<MemberT>() && !std::is_same_v<MemberT, std::string>) { \
            if (node[#member]) { \
                Reflection::deserialize(node[#member], instance.member); \
            } \
        } else { \
            if (node[#member]) { \
                instance.member = Reader::ReadValue<MemberT>(node, #member); \
            } \
        } \
    }}

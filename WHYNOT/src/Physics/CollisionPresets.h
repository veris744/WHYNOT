#pragma once
#include <includes.h>

#include "Reflection/Reflection.h"

enum class ColliderType
{
    Static, Dynamic
};
REGISTER_ENUM(ColliderType,
    {"STATIC", ColliderType::Static},
    {"DYNAMIC", ColliderType::Dynamic}
);

enum class ColliderMode
{
    Query, Physics, All
};
REGISTER_ENUM(ColliderMode,
    {"QUERY", ColliderMode::Query},
    {"PHYSICS", ColliderMode::Physics},
    {"ALL", ColliderMode::All},
);

struct CollisionProfile : ReflectedObject
{
    CollisionProfile() = default;
    CollisionProfile(ColliderType type, ColliderMode mode)
        : type(type), mode(mode) {}
    ColliderType type = ColliderType::Static;
    ColliderMode mode = ColliderMode::Query;
};
REGISTER_CLASS(CollisionProfile, {
    REGISTER_MEMBER(CollisionProfile, type, MemberProperty::NonViewable),
    REGISTER_MEMBER(CollisionProfile, mode, MemberProperty::NonViewable)
});

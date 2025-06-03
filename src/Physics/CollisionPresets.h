#pragma once

#include "Reflection/Reflection.h"

enum class ColliderType
{
    // Static colliders will not be updated in octree
    Static, Dynamic
};
REGISTER_ENUM(ColliderType,
    {"STATIC", ColliderType::Static},
    {"DYNAMIC", ColliderType::Dynamic}
);

enum class ColliderMode
{
    // Query for UI rays, Physics for in game collisions, All for both
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
    CollisionProfile(ColliderType type, ColliderMode mode, bool _isTrigger)
        : type(type), mode(mode), isTrigger(_isTrigger) {}
    ColliderType type = ColliderType::Static;
    ColliderMode mode = ColliderMode::Query;

    // If collider isTrigger collision will generate collisionEvent but not physics impact
    bool isTrigger = false;
};
REGISTER_CLASS(CollisionProfile, {
    REGISTER_MEMBER(CollisionProfile, type, MemberProperty::Viewable | MemberProperty::Serializable),
    REGISTER_MEMBER(CollisionProfile, mode, MemberProperty::Viewable | MemberProperty::Serializable),
    REGISTER_MEMBER(CollisionProfile, isTrigger, MemberProperty::Viewable | MemberProperty::Serializable)
});

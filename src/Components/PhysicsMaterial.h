#pragma once
#include "Component.h"
#include "Reflection/Reflection.h"


class PhysicsMaterial : public Component
{
    static PhysicsMaterial defaultMaterial;

public:
    PhysicsMaterial() = default;
    PhysicsMaterial(float _mass, float _friction, float _bounciness, bool _hasGravity)
        : mass(_mass), friction(_friction), bounciness(_bounciness), hasGravity(_hasGravity) {}

    float mass = 1.0f;
    float friction = 0.2f;
    float bounciness = 0.2f;
    bool hasGravity = true;

    void Update(float deltaTime) override {}

    static PhysicsMaterial* GetDefaultMaterial() { return &defaultMaterial; }
};
REGISTER_CLASS(PhysicsMaterial,
    REGISTER_MEMBER(PhysicsMaterial, mass, MemberProperty::Viewable | MemberProperty::Editable | MemberProperty::Serializable),
    REGISTER_MEMBER(PhysicsMaterial, friction, MemberProperty::Viewable | MemberProperty::Editable | MemberProperty::Serializable),
    REGISTER_MEMBER(PhysicsMaterial, bounciness, MemberProperty::Viewable | MemberProperty::Editable | MemberProperty::Serializable),
    REGISTER_MEMBER(PhysicsMaterial, hasGravity, MemberProperty::Viewable | MemberProperty::Editable | MemberProperty::Serializable)
);

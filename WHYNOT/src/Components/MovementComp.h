#pragma once
#include "Component.h"

class Transform;

class MovementComp : public Component
{
    vec3 speed;
    vec3 acceleration;
    std::shared_ptr<Transform> transform;
    
public:
    MovementComp();
    ~MovementComp() = default;
    
};

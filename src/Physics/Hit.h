#pragma once
#include <glm/vec3.hpp>

using namespace glm;

class Entity;

enum HitType
{
    UI, World
};

struct Hit
{
    bool hasHit = false;
    HitType type;
    vec3 point = vec3(0, 0, 0);
    vec3 normal = vec3(0, 0, 0);
    float distSQ = -1;
    Entity* otherEntity = nullptr;
    Entity* selfEntity = nullptr;
    
    bool operator<(const Hit& other) const {
        return distSQ < other.distSQ;
    }
};

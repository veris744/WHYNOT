#pragma once
#include <glm/vec3.hpp>

class Entity;

enum HitType
{
    UI, WorldHit
};

struct Hit
{
    bool hasHit = false;
    HitType type;
    glm::vec3 point = glm::vec3(0, 0, 0);
    glm::vec3 normal = glm::vec3(0, 0, 0);
    float distSQ = -1;
    Entity* otherEntity = nullptr;
    Entity* selfEntity = nullptr;
    
    bool operator<(const Hit& other) const {
        return distSQ < other.distSQ;
    }

    static void InvertHit(Hit& invertedHit, const Hit& originalHit)
    {
        invertedHit = originalHit;
        invertedHit.selfEntity = originalHit.otherEntity;
        invertedHit.otherEntity = originalHit.selfEntity;
        invertedHit.normal = -originalHit.normal;
    }
};

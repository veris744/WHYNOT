#pragma once
#include <glm/vec3.hpp>

class Entity;

struct Hit
{
    bool hasHit = false;
    glm::vec3 point = glm::vec3(0, 0, 0);
    glm::vec3 normal = glm::vec3(0, 0, 0);
    float distSQ = -1;
    Entity* entity = nullptr;
    
    bool operator<(const Hit& other) const {
        return distSQ < other.distSQ;
    }
};

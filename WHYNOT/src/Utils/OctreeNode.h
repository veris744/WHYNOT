#pragma once
#include <unordered_set>

#include "includes.h"


class Entity;

struct AABB {
    vec3 min;
    vec3 max;
};

class OctreeNode
{
public:
    AABB bounds;
    vector<std::shared_ptr<Entity>> staticEntities;
    vector<std::shared_ptr<Entity>> dynamicEntities;
    std::unique_ptr<OctreeNode> children[8];
    bool isLeaf = true;

    OctreeNode(const AABB& bounds) : bounds(bounds) {}

    void InsertDynamic(const std::shared_ptr<Entity>& entity);
    void InsertStatic(const std::shared_ptr<Entity>& entity);
    void Subdivide();
    void InsertIntoChildrenDynamic(const std::shared_ptr<Entity>& entity);
    void InsertIntoChildrenStatic(const std::shared_ptr<Entity>& entity);
    void QueryCollisions(std::set<pair<std::shared_ptr<Entity>, std::shared_ptr<Entity>>>& collisions);
    void ClearDynamic();
    
};

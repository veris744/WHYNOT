#pragma once
#include <memory>
#include <unordered_set>
#include <set>
#include <glm/glm.hpp>
#include <Utils/Logger.h>

using namespace glm;


struct Hit;
class Entity;

struct AABB {
    vec3 min;
    vec3 max;
    bool IntersectsRay(const vec3& rayOrigin, const vec3& rayDir) const
    {
        vec3 invDir = 1.0f / rayDir;

        vec3 t1 = (min - rayOrigin) * invDir;
        vec3 t2 = (max - rayOrigin) * invDir;

        vec3 tMin = glm::min(t1, t2);
        vec3 tMax = glm::max(t1, t2);

        float tNear = glm::max(glm::max(tMin.x, tMin.y), tMin.z);
        float tFar = glm::min(glm::min(tMax.x, tMax.y), tMax.z);

        return tNear <= tFar && tFar > 0;
    }
};

class OctreeNode
{
    static const int MAX_DEPTH = 8;
    int depth = 0;

public:
    AABB bounds;
    std::vector<std::shared_ptr<Entity>> staticEntities;
    std::vector<std::shared_ptr<Entity>> dynamicEntities;
    std::unique_ptr<OctreeNode> children[8];
    bool isLeaf = true;

    OctreeNode(const AABB& _bounds, int _depth = 0) : bounds(_bounds), depth(_depth)
    { }
    void InsertDynamic(const std::shared_ptr<Entity>& entity);
    void InsertStatic(const std::shared_ptr<Entity>& entity);
    void Subdivide();
    void InsertIntoChildrenDynamic(const std::shared_ptr<Entity>& entity);
    void InsertIntoChildrenStatic(const std::shared_ptr<Entity>& entity);
    void QueryCollisions(std::set<Hit>& collisions);
    void QueryRayCollisions(std::set<Hit>& collisions, vec3 rayStart, vec3 rayDir);
    void ClearDynamic();
    void ClearStatic();
    void Clear();
    
};

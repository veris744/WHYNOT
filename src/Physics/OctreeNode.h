#pragma once
#include <memory>
#include <unordered_set>
#include <set>
#include <glm/glm.hpp>

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
public:
    AABB bounds;
    std::vector<std::shared_ptr<Entity>> staticEntities;
    std::vector<std::shared_ptr<Entity>> dynamicEntities;
    std::unique_ptr<OctreeNode> children[8];
    bool isLeaf = true;

    OctreeNode(const AABB& bounds) : bounds(bounds) {}
    void InsertDynamic(const std::shared_ptr<Entity>& entity);
    void InsertStatic(const std::shared_ptr<Entity>& entity);
    void Subdivide();
    void InsertIntoChildrenDynamic(const std::shared_ptr<Entity>& entity);
    void InsertIntoChildrenStatic(const std::shared_ptr<Entity>& entity);
    void QueryCollisions(std::set<std::pair<std::shared_ptr<Entity>, std::shared_ptr<Entity>>>& collisions);
    void QueryRayCollisions(std::set<Hit>& collisions, vec3 rayStart, vec3 rayDir);
    void ClearDynamic();
    void ClearStatic();
    void Clear();
    
};

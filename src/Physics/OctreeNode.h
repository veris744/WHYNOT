#pragma once
#include <memory>
#include <unordered_set>
#include <set>
#include <glm/glm.hpp>
#include <Utils/Logger.h>

struct Hit;
class Entity;

struct AABB {
    glm::vec3 min;
    glm::vec3 max;
    bool IntersectsRay(const glm::vec3& rayOrigin, const glm::vec3& rayDir) const
    {
        glm::vec3 invDir = 1.0f / rayDir;

        glm::vec3 t1 = (min - rayOrigin) * invDir;
        glm::vec3 t2 = (max - rayOrigin) * invDir;

        glm::vec3 tMin = glm::min(t1, t2);
        glm::vec3 tMax = glm::max(t1, t2);

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
    void QueryRayCollisions(std::set<Hit>& collisions, glm::vec3 rayStart, glm::vec3 rayDir);
    void ClearDynamic();
    void ClearStatic();
    void Clear();
    
};

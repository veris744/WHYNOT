#pragma once
#include "OctreeNode.h"


class CollisionManager
{
    static std::unique_ptr<OctreeNode> root;
    static Hit ThrowRayInEditor(vec3 rayOrigin, vec3 rayDirection);
    
public:
    static void PrepareOctree();
    static void CheckCollisions();

    static Hit ThrowRay(vec3 rayOrigin, vec3 rayDirection, bool showDebug = false, float timer = 3.f);

    static void ClearOctree();
    static void ClearRoot()
    {
        root = nullptr;
    }
    
};

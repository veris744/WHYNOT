#pragma once
#include "OctreeNode.h"


class CollisionManager
{
    static std::unique_ptr<OctreeNode> root;
    
public:
    static void PrepareOctree();
    static void CheckCollisions();
    static bool CheckUIClicked(vec2 mousePos);

    static Hit ThrowRayFromScreen(vec2 mousePos, vec3 playerPosition, bool showDebug = false, float timer = 3.f);
    static Hit ThrowRay(vec3 rayOrigin, vec3 rayDirection, bool showDebug = false, float timer = 3.f);

    static void ClearOctree();
    static void ClearRoot()
    {
        root = nullptr;
    }
    
};

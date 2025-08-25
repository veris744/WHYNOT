#pragma once
#include "OctreeNode.h"


class CollisionManager
{
    static std::unique_ptr<OctreeNode> root;
    
public:
    static void PrepareOctree();
    static void CheckCollisions();
    static bool CheckUIClicked(glm::vec2 mousePos);

    static Hit ThrowRayFromScreen(glm::vec2 mousePos, glm::vec3 playerPosition, bool showDebug = false, float timer = 3.f);
    static Hit ThrowRay(glm::vec3 rayOrigin, glm::vec3 rayDirection, bool showDebug = false, float timer = 3.f);

    static void ClearOctree();
    static void ClearRoot()
    {
        root = nullptr;
    }
    
};

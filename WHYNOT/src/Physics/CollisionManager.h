#pragma once
#include "includes.h"

class OctreeNode;

class CollisionManager
{
    static std::unique_ptr<OctreeNode> root;
public:
    static void PrepareOctree();
    static void CheckCollisions();
    
};

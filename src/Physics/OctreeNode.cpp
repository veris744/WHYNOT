#include "OctreeNode.h"

#include <Utils/Parser.h>

#include "Hit.h"
#include "Components/Colliders/Collider.h"
#include "Entities/Entity.h"

AABB playerbounds;

void OctreeNode::InsertDynamic(const std::shared_ptr<Entity>& entity)
{
    if (!isLeaf || depth >= MAX_DEPTH) {
        if (!isLeaf) {
            InsertIntoChildrenDynamic(entity);
        } else {
            dynamicEntities.push_back(entity);
        }
        return;
    }

    if (dynamicEntities.size() + staticEntities.size() < 4) {
        dynamicEntities.push_back(entity);
        return;
    }
    Subdivide();

    auto oldDynamic = std::move(dynamicEntities);
    auto oldStatic = std::move(staticEntities);

    for (const auto& e : oldDynamic) {
        InsertIntoChildrenDynamic(e);
    }
    for (const auto& e : oldStatic) {
        InsertIntoChildrenStatic(e);
    }

    InsertIntoChildrenDynamic(entity);
}

void OctreeNode::InsertStatic(const std::shared_ptr<Entity>& entity)
{
    if (!isLeaf || depth >= MAX_DEPTH) {
        if (!isLeaf) {
            InsertIntoChildrenStatic(entity);
        } else {
            staticEntities.push_back(entity);
        }
        return;
    }

    if (dynamicEntities.size() + staticEntities.size() < 4) {
        staticEntities.push_back(entity);
        return;
    }

    Subdivide();

    auto oldDynamic = std::move(dynamicEntities);
    auto oldStatic = std::move(staticEntities);

    for (const auto& e : oldDynamic) {
        InsertIntoChildrenDynamic(e);
    }
    for (const auto& e : oldStatic) {
        InsertIntoChildrenStatic(e);
    }

    InsertIntoChildrenStatic(entity);
}

void OctreeNode::Subdivide()
{
    if (!isLeaf) return;

    isLeaf = false;
    vec3 size = (bounds.max - bounds.min) * 0.5f;

    for (int i = 0; i < 8; i++) {
        vec3 offset = {
            (i & 1) ? size.x : 0,
            (i & 2) ? size.y : 0,
            (i & 4) ? size.z : 0
        };
        AABB childBounds{
            bounds.min + offset,
            bounds.min + offset + size
        };
        children[i] = std::make_unique<OctreeNode>(childBounds, depth + 1);
    }
}

void OctreeNode::InsertIntoChildrenDynamic(const std::shared_ptr<Entity>& entity)
{
    bool inserted = false;
    Collider* collider = entity->GetComponent<Collider>();
    if (collider) {
        for (const auto& child : children) {
            vec3 min = child->bounds.min;
            vec3 max = child->bounds.max;

            if (collider->OverlapsBounds({min.x, max.x}, {min.y, max.y}, {min.z, max.z})) {
                child->InsertDynamic(entity);
                inserted = true;
            }
        }
    }

    if (!inserted) {
        dynamicEntities.push_back(entity);
    }
}

void OctreeNode::InsertIntoChildrenStatic(const std::shared_ptr<Entity>& entity)
{
    bool inserted = false;
    Collider* collider = entity->GetComponent<Collider>();
    if (collider) {
        for (const auto& child : children) {
            vec3 min = child->bounds.min;
            vec3 max = child->bounds.max;

            if (collider->OverlapsBounds({min.x, max.x}, {min.y, max.y}, {min.z, max.z})) {
                child->InsertStatic(entity);
                inserted = true;
                // Don't return here - entity might belong to multiple children
            }
        }
    }

    if (!inserted) {
        staticEntities.push_back(entity);
    }
}

void OctreeNode::QueryCollisions(std::set<Hit>& collisions)
{
    if (isLeaf) {
        for (size_t i = 0; i < dynamicEntities.size(); i++) {
            const auto& e1 = dynamicEntities[i];
            if (!e1 || !e1->isActive) continue;
            
            for (size_t j = i + 1; j < dynamicEntities.size(); j++) {
                const auto& e2 = dynamicEntities[j];
                if (!e2 || !e2->isActive) continue;

                Collider* c1 = e1->GetComponent<Collider>();
                Collider* c2 = e2->GetComponent<Collider>();
                Hit hit;
                if (c1 && c2 && c1->Collides(c2, hit)) {
                    collisions.insert(hit);
                }
            }
            
            for (const auto& e2 : staticEntities) {
                if (!e2 || !e2->isActive) continue;

                Collider* c1 = e1->GetComponent<Collider>();
                Collider* c2 = e2->GetComponent<Collider>();
                Hit hit;
                if (c1 && c2 && c1->Collides(c2, hit)) {
                    collisions.insert(hit);
                }
            }
        }
    } else {
        for (const auto& child : children) {
            child->QueryCollisions(collisions);
        }
    }
}

void OctreeNode::QueryRayCollisions(std::set<Hit>& collisions, vec3 rayStart, vec3 rayDir)
{
    if (!bounds.IntersectsRay(rayStart, rayDir)) {
        return;
    }

    if (isLeaf) {
        for (const auto& entity : dynamicEntities) {
            if (!entity || !entity->isActive) continue;
            Collider* collider = entity->GetComponent<Collider>();
            Hit hit;
            collider->RayCollides(rayStart, rayDir, hit);
            if (hit.hasHit)
            {
                collisions.insert(hit);
            }
        }
        
        for (const auto& entity : staticEntities) {
            if (!entity || !entity->isActive) continue;
            Collider* collider = entity->GetComponent<Collider>();
            Hit hit;
            collider->RayCollides(rayStart, rayDir, hit);
            if (hit.hasHit)
            {
                collisions.insert(hit);
            }
        }
    } 
    else {
        for (const auto& child : children) {
            if (child) {
                child->QueryRayCollisions(collisions, rayStart, rayDir);
            }
        }
    }
}

void OctreeNode::ClearDynamic()
{
    dynamicEntities.clear();
    if (!isLeaf)
    {
        for (const auto& child : children) {
            child->ClearDynamic();
        }
    }
}

void OctreeNode::ClearStatic()
{
    if (isLeaf)
    {
        staticEntities.clear();
    }
    else
    {
        for (const auto& child : children) {
            child->ClearStatic();
        }
    }
}

void OctreeNode::Clear()
{
    ClearDynamic();
    ClearStatic();
}

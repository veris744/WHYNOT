#include "OctreeNode.h"

#include "Components/Collider.h"
#include "Entities/Entity.h"


void OctreeNode::InsertDynamic(const std::shared_ptr<Entity>& entity)
{
    if (isLeaf && dynamicEntities.size() + staticEntities.size() < 4) {
        dynamicEntities.push_back(entity);
        return;
    }

    if (isLeaf) {
        Subdivide();
        std::vector<std::shared_ptr<Entity>> oldEntities = std::move(dynamicEntities);
        for (const auto& e : oldEntities) {
            InsertIntoChildrenDynamic(e);
        }
    }
    InsertIntoChildrenDynamic(entity);
}

void OctreeNode::InsertStatic(const std::shared_ptr<Entity>& entity)
{
    if (isLeaf && dynamicEntities.size() + staticEntities.size() < 4) {
        staticEntities.push_back(entity);
        return;
    }

    if (isLeaf) {
        Subdivide();
        std::vector<std::shared_ptr<Entity>> oldEntities = std::move(staticEntities);
        for (const auto& e : oldEntities) {
            InsertIntoChildrenStatic(e);
        }
    }
    InsertIntoChildrenStatic(entity);
}

void OctreeNode::Subdivide()
{
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
        children[i] = std::make_unique<OctreeNode>(childBounds);
    }
}

void OctreeNode::InsertIntoChildrenDynamic(const std::shared_ptr<Entity>& entity)
{
    Collider* collider = entity->GetComponent<Collider>();
    if (collider) {
        for (const auto& child : children) {
            vec3 min = child->bounds.min;
            vec3 max = child->bounds.max;

            if (collider->OverlapsBounds({min.x, max.x}, {min.y, max.y}, {min.z, max.z})) {
                child->InsertDynamic(entity);
                return;
            }
        }
    }
    dynamicEntities.push_back(entity);
}

void OctreeNode::InsertIntoChildrenStatic(const std::shared_ptr<Entity>& entity)
{
    Collider* collider = entity->GetComponent<Collider>();
    if (collider) {
        for (const auto& child : children) {
            vec3 min = child->bounds.min;
            vec3 max = child->bounds.max;

            if (collider->OverlapsBounds({min.x, max.x}, {min.y, max.y}, {min.z, max.z})) {
                child->InsertStatic(entity);
                return;
            }
        }
    }
    staticEntities.push_back(entity);
}


void OctreeNode::QueryCollisions(std::set<pair<std::shared_ptr<Entity>, std::shared_ptr<Entity>>>& collisions)
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
                if (c1 && c2 && c1->Collides(c2)) {
                    collisions.insert({e1, e2});
                }
            }
            
            for (const auto& e2 : staticEntities) {
                if (!e2 || !e2->isActive) continue;

                Collider* c1 = e1->GetComponent<Collider>();
                Collider* c2 = e2->GetComponent<Collider>();
                if (c1 && c2 && c1->Collides(c2)) {
                    collisions.insert({e1, e2});
                }
            }
        }
    } else {
        for (const auto& child : children) {
            child->QueryCollisions(collisions);
        }
    }
}

void OctreeNode::ClearDynamic()
{
    if (isLeaf)
    {
        dynamicEntities.clear();
    }
    else
    {
        for (const auto& child : children) {
            child->ClearDynamic();
        }
    }
}

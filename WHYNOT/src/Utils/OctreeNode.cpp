#include "OctreeNode.h"

#include "Components/Collider.h"
#include "Entities/Entity.h"


void OctreeNode::Insert(const std::shared_ptr<Entity>& entity)
{
    if (isLeaf && entities.size() < 4) {
        entities.push_back(entity);
        return;
    }

    if (isLeaf) {
        Subdivide();
        std::vector<std::shared_ptr<Entity>> oldEntities = std::move(entities);
        for (auto& e : oldEntities) {
            InsertIntoChildren(e);
        }
    }
    InsertIntoChildren(entity);
    
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

void OctreeNode::InsertIntoChildren(const std::shared_ptr<Entity>& entity)
{
    auto collider = entity->GetComponent<Collider>();
    if (collider) {
        for (auto& child : children) {
            vec3 min = child->bounds.min;
            vec3 max = child->bounds.max;

            if (collider->OverlapsBounds({min.x, max.x}, {min.y, max.y}, {min.z, max.z})) {
                child->Insert(entity);
                return;
            }
        }
    }
    entities.push_back(entity);
}

void OctreeNode::QueryCollisions(std::set<pair<std::shared_ptr<Entity>, std::shared_ptr<Entity>>>& collisions)
{
    if (isLeaf) {
        for (size_t i = 0; i < entities.size(); i++) {
            auto& e1 = entities[i];
            if (!e1 || !e1->isActive) continue;
            
            for (size_t j = i + 1; j < entities.size(); j++) {
                auto& e2 = entities[j];
                if (!e2 || !e2->isActive) continue;

                auto c1 = e1->GetComponent<Collider>();
                auto c2 = e2->GetComponent<Collider>();
                if (c1 && c2 && c1->Collides(c2)) {
                    collisions.insert({e1, e2});
                }
            }
        }
    } else {
        for (auto& child : children) {
            child->QueryCollisions(collisions);
        }
    }
}
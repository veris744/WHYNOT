#include "CollisionManager.h"

#include "Components/Collider.h"
#include "Managers/Helper.h"
#include "Managers/World.h"
#include "Utils/OctreeNode.h"

std::unique_ptr<OctreeNode> CollisionManager::root = nullptr;

void CollisionManager::PrepareOctree()
{
    AABB worldBounds = {vec3(Helper::GetXBounds().x, Helper::GetYBounds().x, Helper::GetZBounds().x), 
        vec3(Helper::GetXBounds().y, Helper::GetYBounds().y, Helper::GetZBounds().y)};
    
    root = std::make_unique<OctreeNode>(worldBounds);
    
    for (const auto& [name, entity] : World::GetInstance()->GetEntities())
    {
        Collider* collider = entity->GetComponent<Collider>();
        if (entity && entity->isActive && entity->HasCollision() && collider->profile.type == ColliderType::Static
            && (collider->profile.mode == ColliderMode::Physics || collider->profile.mode == ColliderMode::All))
        {
            root->InsertStatic(entity);
        }
    }
}

void CollisionManager::CheckCollisions()
{
    for (auto& [id, entity] :  World::GetInstance()->GetEntities()) {
        Collider* collider = entity->GetComponent<Collider>();
        if (entity && entity->isActive && entity->HasCollision() && collider->profile.type == ColliderType::Dynamic
            && (collider->profile.mode == ColliderMode::Physics || collider->profile.mode == ColliderMode::All))
        {
            root->InsertDynamic(entity);
        }
    }
    
    std::set<pair<std::shared_ptr<Entity>, std::shared_ptr<Entity>>> collisions;
    root->QueryCollisions(collisions);
    
    for (const auto& collisionPair  : collisions) {
        const auto& e1 = collisionPair.first;
        const auto& e2 = collisionPair.second;
        
        if (!e1->isActive || !e2->isActive) continue;
        
        auto c1 = e1->GetComponent<Collider>();
        auto c2 = e2->GetComponent<Collider>();
        if (c1->Collides(c2)) {
            vec3 normal = normalize(c1->GetWorldPosition() - c2->GetWorldPosition());
            c1->CollisionDelegate.Execute(e2, normal);
            c2->CollisionDelegate.Execute(e1, -normal);
        }
    }

    root->ClearDynamic();
}

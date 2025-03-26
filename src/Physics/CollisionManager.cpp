#include "CollisionManager.h"

#include <Managers/ConfigurationValues.h>

#include "Hit.h"
#include "Components/Collider.h"
#include "Managers/Helper.h"
#include "Managers/World.h"
#include "OctreeNode.h"
#include "Input/InputManager.h"
#include "Utils/Debugger.h"

std::unique_ptr<OctreeNode> CollisionManager::root = nullptr;

void CollisionManager::PrepareOctree()
{
    if (!root)
    {
        AABB worldBounds = {vec3(Helper::GetXBounds().x, Helper::GetYBounds().x, Helper::GetZBounds().x), 
            vec3(Helper::GetXBounds().y, Helper::GetYBounds().y, Helper::GetZBounds().y)};
    
        root = std::make_unique<OctreeNode>(worldBounds);
    }
    
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
    if (!root)  return;
    
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

Hit CollisionManager::ThrowRay(vec3 rayOrigin, vec3 rayDirection, bool showDebug, float timer)
{
    if (showDebug)
    {
        Debugger::DrawLineDebug(rayOrigin, rayOrigin + 20.f * rayDirection, vec3(1,0,0), timer);
    }
    if (ConfigurationValues::IsEditorOpen)
    {
        return ThrowRayInEditor(rayOrigin, rayDirection);
    }
    return {false};
}

Hit CollisionManager::ThrowRayInEditor(vec3 rayOrigin, vec3 rayDirection)
{
    if (!root)
    {
        AABB worldBounds = {vec3(Helper::GetXBounds().x, Helper::GetYBounds().x, Helper::GetZBounds().x), 
            vec3(Helper::GetXBounds().y, Helper::GetYBounds().y, Helper::GetZBounds().y)};
        root = std::make_unique<OctreeNode>(worldBounds);
    }
    
    for (auto& [id, entity] :  World::GetInstance()->GetEntities()) {
        Collider* collider = entity->GetComponent<Collider>();
        if (entity && entity->isActive && entity->HasCollision()
            && (collider->profile.mode == ColliderMode::Query || collider->profile.mode == ColliderMode::All))
        {
            if (collider->profile.type == ColliderType::Dynamic)
            {
                root->InsertDynamic(entity);
            }
            else
            {
                root->InsertStatic(entity);
            }
        }
    }

    set<Hit> hits;
    root->QueryRayCollisions(hits, rayOrigin, rayDirection);

    float minDistanceSQ = -1;
    Hit closestHit;
    for (const auto& hit : hits)
    {
        float distSQ = hit.distSQ;
        if (distSQ < minDistanceSQ || minDistanceSQ == -1)
        {
            minDistanceSQ = distSQ;
            closestHit = hit;
        }
    }
    
    root->Clear();
    
    return closestHit;
}

void CollisionManager::ClearOctree()
{
    if (root)
    {
        root->Clear();
    }
}

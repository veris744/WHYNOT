#include "CollisionManager.h"

#include <Managers/ConfigurationValues.h>
#include <UI/Widget.h>

#include "Hit.h"
#include "Components/Collider.h"
#include "Managers/Helper.h"
#include "Managers/World.h"
#include "OctreeNode.h"
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

bool CollisionManager::CheckUIClicked(vec2 mousePos)
{
    bool isHitting = false;
    for (const auto& widget : World::GetInstance()->GetWidgetsChildOf())
    {
        if (widget->isBlocking && widget->AreParentsActive())
        {
            if (widget->IsClicking(mousePos))
            {
                widget->OnClick(mousePos);
                isHitting = true;
            }
        }
    }
    return isHitting;
}

Hit CollisionManager::ThrowRayFromScreen(vec2 mousePos, vec3 playerPosition, bool showDebug, float timer)
{
    if (CheckUIClicked(mousePos))
    {
        return Hit{true, HitType::UI, vec3(mousePos, 0)};
    }

    // Convert mouse position to NDC
    float x = (2.0f * mousePos.x) / Helper::windowWidth - 1.0f;
    float y = 1.0f - (2.0f * mousePos.y) / Helper::windowHeight;
    vec4 rayClip = vec4(x, y, -1.0f, 1.0f);

    // Convert from clip space to view space
    mat4 invProj = inverse(World::GetInstance()->GetCurrentCamera()->GetProjectionMatrix());
    vec4 rayEye = invProj * rayClip;
    rayEye = vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

    // Convert from view space to world space
    mat4 invView = inverse(World::GetInstance()->GetCurrentCamera()->GetViewMatrix());
    vec4 rayWorld = invView * rayEye;
    vec3 rayDir = normalize(vec3(rayWorld));

    vec3 mousePos3D = playerPosition + rayDir;

    return ThrowRay(playerPosition, mousePos3D - playerPosition, showDebug, timer);
}

Hit CollisionManager::ThrowRay(vec3 rayOrigin, vec3 rayDirection, bool showDebug, float timer)
{
    if (showDebug)
    {
        Debugger::DrawLineDebug(rayOrigin, rayOrigin + 20.f * rayDirection, vec3(1,0,0), timer);
    }

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

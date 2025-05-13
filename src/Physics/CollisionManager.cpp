#include "CollisionManager.h"

#include <Managers/ConfigurationValues.h>
#include <UI/Widget.h>
#include <Utils/Parser.h>

#include "Hit.h"
#include "Components/Colliders/Collider.h"
#include "Managers/Helper.h"
#include "Managers/World.h"
#include "OctreeNode.h"
#include "Utils/Debugger.h"

std::unique_ptr<OctreeNode> CollisionManager::root = nullptr;


void CollisionManager::PrepareOctree()
{
    Logger::Log(LogLevel::Info, "PREPARING");
    if (!root)
    {
        AABB worldBounds = {vec3(Helper::GetXBounds().x, Helper::GetYBounds().x, Helper::GetZBounds().x), 
            vec3(Helper::GetXBounds().y, Helper::GetYBounds().y, Helper::GetZBounds().y)};
    
        root = std::make_unique<OctreeNode>(worldBounds);
    }
    else
    {
        root->Clear();
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
    Logger::Log(LogLevel::Info, Parser::Parse((int)root->staticEntities.size()));
}

void CollisionManager::CheckCollisions()
{
    if (!root)  return;
    if (World::GetInstance()->IsPaused())  return;
    
    for (auto& [id, entity] :  World::GetInstance()->GetEntities()) {
        Collider* collider = entity->GetComponent<Collider>();
        if (entity && entity->isActive && entity->HasCollision() && collider->profile.type == ColliderType::Dynamic
            && (collider->profile.mode == ColliderMode::Physics || collider->profile.mode == ColliderMode::All))
        {
            root->InsertDynamic(entity);
        }
    }
    
    std::set<Hit> collisions;
    root->QueryCollisions(collisions);
    
    for (const auto& hit  : collisions) {
        const auto& e1 = hit.selfEntity;
        const auto& e2 = hit.otherEntity;
        
        if (!e1 || !e1->isActive) continue;
        if (!e2 || !e2->isActive) continue;

        const auto c1 = e1->GetComponent<Collider>();
        const auto c2 = e2->GetComponent<Collider>();
        if (hit.hasHit) {
            c1->CollisionDelegate.Execute(hit.otherEntity, hit.normal);
            c1->UpdateMovement(hit);
            if (e2->isActive)
            {
                Hit hit2;
                Hit::InvertHit(hit2, hit);
                c2->CollisionDelegate.Execute(hit2.selfEntity, hit2.normal);
                c2->UpdateMovement(hit2);
            }
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

    return ThrowRay(playerPosition, (mousePos3D - playerPosition) * 10.f, showDebug, timer);
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

    root->Clear();
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

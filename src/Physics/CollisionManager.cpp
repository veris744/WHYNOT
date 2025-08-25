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
    Logger::Log(LogLevel::Info, "PREPARING OCTREE");
    if (!root)
    {
        AABB worldBounds = {glm::vec3(Helper::GetXBounds().x, Helper::GetYBounds().x, Helper::GetZBounds().x), 
            glm::vec3(Helper::GetXBounds().y, Helper::GetYBounds().y, Helper::GetZBounds().y)};
    
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
            Hit hit2;
            Hit::InvertHit(hit2, hit);

            if (!c1->profile.isTrigger && !c2->profile.isTrigger)
            {
                c1->UpdateMovement(hit);
                c2->UpdateMovement(hit2);
            }

            c1->CollisionDelegate.Execute(hit.otherEntity, hit.normal);
            c2->CollisionDelegate.Execute(hit2.otherEntity, hit2.normal);
            //Logger::Log(LogLevel::Info, "Collision: " + e1->name + " : " + e2->name);
        }
    }
    root->ClearDynamic();
}

bool CollisionManager::CheckUIClicked(glm::vec2 mousePos)
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

Hit CollisionManager::ThrowRayFromScreen(glm::vec2 mousePos, glm::vec3 playerPosition, bool showDebug, float timer)
{
    if (CheckUIClicked(mousePos))
    {
        return Hit{true, HitType::UI, glm::vec3(mousePos, 0)};
    }

    // Convert mouse position to NDC
    float x = (2.0f * mousePos.x) / Helper::windowWidth - 1.0f;
    float y = 1.0f - (2.0f * mousePos.y) / Helper::windowHeight;
    glm::vec4 rayClip = glm::vec4(x, y, -1.0f, 1.0f);

    // Convert from clip space to view space
    glm::mat4 invProj = inverse(World::GetInstance()->GetCurrentCamera()->GetProjectionMatrix());
    glm::vec4 rayEye = invProj * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

    // Convert from view space to world space
    glm::mat4 invView = inverse(World::GetInstance()->GetCurrentCamera()->GetViewMatrix());
    glm::vec4 rayWorld = invView * rayEye;
    glm::vec3 rayDir = normalize(glm::vec3(rayWorld));

    glm::vec3 mousePos3D = playerPosition + rayDir;

    return ThrowRay(playerPosition, (mousePos3D - playerPosition) * 10.f, showDebug, timer);
}

Hit CollisionManager::ThrowRay(glm::vec3 rayOrigin, glm::vec3 rayDirection, bool showDebug, float timer)
{
    if (showDebug)
    {
        Debugger::DrawLineDebug(rayOrigin, rayOrigin + 20.f * rayDirection, glm::vec3(1,0,0), timer);
    }

    if (!root)
    {
        AABB worldBounds = {glm::vec3(Helper::GetXBounds().x, Helper::GetYBounds().x, Helper::GetZBounds().x), 
            glm::vec3(Helper::GetXBounds().y, Helper::GetYBounds().y, Helper::GetZBounds().y)};
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

    std::set<Hit> hits;
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

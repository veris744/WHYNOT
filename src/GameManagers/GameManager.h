#pragma once
#include <glm.hpp>
#include <Entities/Player.h>
#include <Utils/SingleDelegate.h>

class Entity;

class GameManager
{
protected:
    glm::vec3 playgroundDimensions = glm::vec3(12,12,6);
    glm::vec3 playgroundCenter = glm::vec3(0,0,-8);
    glm::vec3 playerStart = glm::vec3(0,0,0);
    glm::vec3 playerStartRotation = glm::vec3(0,0,0);

    Entity* player = nullptr;

public:
    GameManager() = default;
    virtual ~GameManager() = default;
    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;

    virtual void PrepareGame() = 0;
    virtual void StartGame() = 0;
    virtual void EndGame() = 0;
    virtual void SetPlayer() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void ProcessInput(int key, bool press) = 0;

    glm::vec2 GetXBounds() const{
        return glm::vec2(playgroundCenter.z - playgroundDimensions.z * 0.5f, playgroundCenter.z + playgroundDimensions.z * 0.5f);
    }
    glm::vec2 GetYBounds() const{
        return glm::vec2(playgroundCenter.y - playgroundDimensions.y * 0.5f, playgroundCenter.y + playgroundDimensions.y * 0.5f);
    }
    glm::vec2 GetZBounds() const{
        return glm::vec2(playgroundCenter.z - playgroundDimensions.z * 0.5f, playgroundCenter.z + playgroundDimensions.z * 0.5f);
    }

    glm::vec3 GetPlayerStart() const {  return playerStart; }
    glm::vec3 GetPlayerStartRotation() const {  return playerStartRotation; }
    Entity* GetPlayer() const { return player; }

    SingleDelegate<> OnPlayerShootDelegate;
};

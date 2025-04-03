#pragma once
#include <glm.hpp>
#include <Utils/SingleDelegate.h>

using namespace glm;

class GameManager
{
    vec3 playgroundDimensions = vec3(12,12,6);
    vec3 playgroundCenter = vec3(0,0,-8);
    vec3 playerStart = vec3(0,0,0);

public:
    GameManager() = default;
    virtual ~GameManager() = default;
    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;

    virtual void PrepareGame() = 0;
    virtual void StartGame() = 0;
    virtual void EndGame() = 0;

    vec2 GetXBounds() const{
        return vec2(playgroundCenter.z - playgroundDimensions.z * 0.5f, playgroundCenter.z + playgroundDimensions.z * 0.5f);
    }
    vec2 GetYBounds() const{
        return vec2(playgroundCenter.y - playgroundDimensions.y * 0.5f, playgroundCenter.y + playgroundDimensions.y * 0.5f);
    }
    vec2 GetZBounds() const{
        return vec2(playgroundCenter.z - playgroundDimensions.z * 0.5f, playgroundCenter.z + playgroundDimensions.z * 0.5f);
    }

    vec3 GetPlayerStart() const {  return playerStart; }

    SingleDelegate<> OnPlayerShootDelegate;
};

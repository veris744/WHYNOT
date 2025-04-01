#pragma once
#include <glm.hpp>
#include <Reflection/ReflectedObject.h>

using namespace glm;

template <typename T>
class GameManager : ReflectedObject
{
    vec3 playgroundDimensions = vec3(12,12,6);
    vec3 playgroundCenter = vec3(0,0,-8);

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

    static T& GetInstance() {
        static T instance;
        return instance;
    }
};

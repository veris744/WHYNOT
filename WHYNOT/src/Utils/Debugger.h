#pragma once
#include <includes.h>

class Mesh;

class Debugger
{
    static unordered_map<std::unique_ptr<Mesh>, mat4> meshesToRender;

    static bool collisionDebugEnabled;
    
public:
    static void SetCollisionDebug(bool isEnabled);
    static bool GetCollisionDebugEnabled() { return collisionDebugEnabled; }
    static void DrawSphereDebug(float _radius, vec3 _position, vec3 _color = vec3(1, 0, 0));
    static void DrawLineDebug(vec3 _start, vec3 _end, vec3 _color = vec3(1, 0, 0));

    static void Render();
    static void Clear();
    
};

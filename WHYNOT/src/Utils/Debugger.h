#pragma once
#include <includes.h>

class Mesh;

class Debugger
{
    static std::shared_ptr<Mesh> mesh;

    static void SetSphereMesh();
    
public:
    static bool collisionDebugEnabled;
    static void DrawSphereDebug(float _radius, vec3 _position, vec3 _color = vec3(1, 0, 0));
    
};

#pragma once
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

class Widget;
class Mesh;

using namespace std;
using namespace glm;

class Debugger
{
    static unordered_map<std::unique_ptr<Mesh>, mat4> meshesToRenderInFrame;
    static map<unique_ptr<Mesh>, mat4> meshesToRender;

    static bool collisionDebugEnabled;
    static bool transformDebugEnabled;
    
public:
    static void SetCollisionDebug(bool isEnabled);
    static void SetTransfomDebug(bool isEnabled);
    static bool GetCollisionDebugEnabled() { return collisionDebugEnabled; }
    static bool GetTransformDebugEnabled() { return collisionDebugEnabled; }
    
    static void DrawSphereDebug(float _radius, vec3 _position, vec3 _color = vec3(1, 0, 0), float timer = 0.f);
    static void DrawCubeDebug(vec3 _dimensions, vec3 _position, vec3 _color = vec3(1, 0, 0), float timer = 0.f);
    static void DrawCapsuleDebug(float _radius, float _height, vec3 _position, vec3 _color = vec3(1, 0, 0), float timer = 0.f);
    static void DrawLineDebug(vec3 _start, vec3 _end, vec3 _color = vec3(1, 0, 0), float timer = 0.f);
    static void DrawTextDebug(const string& _text, vec3 color = vec3(1, 0, 0), float timer = 0.f);
    static void ProcessInput(int key);

    static void Render();
    static void Clear();

    static void StopRenderingMesh(Mesh* mesh);
    static void StopRenderingWidget(Widget* _widget);

    static vector<shared_ptr<Widget>> widgetsToRenderInFrame;
    static vector<shared_ptr<Widget>> widgetsToRender;
};

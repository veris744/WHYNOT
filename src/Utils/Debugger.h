#pragma once
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

class Widget;
class Mesh;

class Debugger
{
    static std::unordered_map<std::unique_ptr<Mesh>, glm::mat4> meshesToRenderInFrame;
    static std::map<std::unique_ptr<Mesh>, glm::mat4> meshesToRender;

    static bool collisionDebugEnabled;
    static bool transformDebugEnabled;
    
public:
    static void SetCollisionDebug(bool isEnabled);
    static void SetTransfomDebug(bool isEnabled);
    static bool GetCollisionDebugEnabled() { return collisionDebugEnabled; }
    static bool GetTransformDebugEnabled() { return collisionDebugEnabled; }
    
    static void DrawSphereDebug(float _radius, glm::vec3 _position, glm::vec3 _color = glm::vec3(1, 0, 0), float timer = 0.f);
    static void DrawCubeDebug(glm::vec3 _dimensions, glm::vec3 _position, glm::vec3 _color = glm::vec3(1, 0, 0), float timer = 0.f);
    static void DrawPlaneDebug(glm::vec2 _dimensions, glm::vec3 _position, glm::vec3 _color = glm::vec3(1, 0, 0), float timer = 0.f);
    static void DrawSlopeDebug(glm::vec3 _dimensions, glm::vec3 _position, glm::vec3 _color = glm::vec3(1, 0, 0), float timer = 0.f);
    static void DrawCapsuleDebug(float _radius, float _height, glm::vec3 _position, glm::vec3 _color = glm::vec3(1, 0, 0), float timer = 0.f);
    static void DrawLineDebug(glm::vec3 _start, glm::vec3 _end, glm::vec3 _color = glm::vec3(1, 0, 0), float timer = 0.f);
    static void DrawTextDebug(const std::string& _text, glm::vec3 color = glm::vec3(1, 0, 0), float timer = 0.f);
    static void ProcessInput(int key);

    static void Render();
    static void Clear();

    static void StopRenderingMesh(Mesh* mesh);
    static void StopRenderingWidget(Widget* _widget);

    static std::vector<std::shared_ptr<Widget>> widgetsToRenderInFrame;
    static std::vector<std::shared_ptr<Widget>> widgetsToRender;
};

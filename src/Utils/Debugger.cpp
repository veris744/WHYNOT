#include "Debugger.h"

#include <Managers/Renderer.h>
#include <UI/Text/Text.h>

#include "Parser.h"
#include "Timer.h"
#include "Components/Collider.h"
#include "Graphics/Mesh.h"
#include "Components/Model.h"
#include "Graphics/Material.h"
#include "Managers/Helper.h"
#include "Managers/World.h"

bool Debugger::collisionDebugEnabled = false;
unordered_map<std::unique_ptr<Mesh>, mat4> Debugger::meshesToRenderInFrame;
map<std::unique_ptr<Mesh>, mat4> Debugger::meshesToRender;
vector<shared_ptr<Widget>> Debugger::widgetsToRenderInFrame;
vector<shared_ptr<Widget>> Debugger::widgetsToRender;


void Debugger::SetCollisionDebug(bool isEnabled)
{
    collisionDebugEnabled = isEnabled;
    for (const auto& [name, entity] : World::GetInstance()->GetEntities())
    {
        if (entity->HasCollision())
        {
            entity->debugEnabled = isEnabled;
            entity->GetComponent<Collider>()->debugEnabled = true;
        }
    }
}

void Debugger::DrawSphereDebug(float _radius, vec3 _position, vec3 _color, float timer)
{
    vector<float> vertices;
    vector<unsigned int> indices;
    Helper::generateSphere(vertices, indices, 1, 16, 8);
    std::shared_ptr<Material> material = std::make_shared<Material>("", DEFAULT_VERTEX_SHADER_PATH, "shaders/debugFragment.glsl");
    std::unique_ptr<Mesh> sphereMesh = std::make_unique<Mesh>(vertices, vertices.size(), indices, material);
    
    sphereMesh->GetMaterial()->materialData.type = MaterialType::COLOR;
    sphereMesh->GetMaterial()->materialData.color = vec4(_color, 0.4);
    mat4 mat = mat4(1.0f);
    mat = translate(mat, _position);
    mat = scale(mat, vec3(_radius));

    if (timer <= 0.f)
    {
        meshesToRenderInFrame[std::move(sphereMesh)] = mat;
    }
    else
    {
        Timer::StartTimer(timer, &Debugger::StopRenderingMesh, sphereMesh.get());
        meshesToRender[std::move(sphereMesh)] = mat;
    }
}

void Debugger::DrawCubeDebug(vec3 _dimensions, vec3 _position, vec3 _color, float timer)
{
    vector<float> vertices = Renderer::GetCubeVertex();
    std::shared_ptr<Material> material = std::make_shared<Material>("", DEFAULT_VERTEX_SHADER_PATH, "shaders/debugFragment.glsl");
    std::unique_ptr<Mesh> cubeMesh = std::make_unique<Mesh>(vertices, 36, material);

    cubeMesh->GetMaterial()->materialData.type = MaterialType::COLOR;
    cubeMesh->GetMaterial()->materialData.color = vec4(_color, 0.4);
    mat4 mat = mat4(1.0f);
    mat = translate(mat, _position);
    mat = scale(mat, _dimensions);

    if (timer <= 0.f)
    {
        meshesToRenderInFrame[std::move(cubeMesh)] = mat;
    }
    else
    {
        Timer::StartTimer(timer, &Debugger::StopRenderingMesh, cubeMesh.get());
        meshesToRender[std::move(cubeMesh)] = mat;
    }
}

void Debugger::DrawLineDebug(vec3 _start, vec3 _end, vec3 _color, float timer)
{
    if (meshesToRender.size() >= 10) return;
    vector<float> vertices =
        {_start.x, _start.y, _start.z, 
        _end.x, _end.y, _end.z};
    std::shared_ptr<Material> material = std::make_shared<Material>("", DEFAULT_VERTEX_SHADER_PATH, "shaders/debugFragment.glsl");
    material->materialData.type = MaterialType::COLOR;
    material->materialData.color = vec4(_color, 1);
    std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>(vertices, 2, material, 1);
    mesh->SetLinesVertexArray();
    
    mat4 mat = mat4(1.0f);
    mesh->Render(mat);

    if (timer <= 0.f)
    {
        meshesToRenderInFrame[std::move(mesh)] = mat;
    }
    else
    {
        Timer::StartTimer(timer, &Debugger::StopRenderingMesh, mesh.get());
        meshesToRender[std::move(mesh)] = mat;
    }
}

void Debugger::DrawTextDebug(const string& _text, vec3 _color, float timer)
{
    std::shared_ptr<Text> text = std::make_shared<Text>(_text, _color, 0.3, vec2{0, 0});
    text->align = TextAlign::RIGHT;
    text->alignVertical = TextAlignVertical::BOTTOM;
    text->padding = {5, 5};
    text->Initialize();
    if (timer <= 0.f)
    {
        widgetsToRenderInFrame.push_back(text);
    }
    else
    {
        Timer::StartTimer(timer, &Debugger::StopRenderingWidget, static_cast<Widget*>(text.get()));
        widgetsToRender.push_back(text);
    }
}

void Debugger::Render()
{
    for (const auto& [mesh, mat] : meshesToRender)
    {
        mesh->Render(mat);
    }
    for (const auto& [mesh, mat] : meshesToRenderInFrame)
    {
        mesh->Render(mat);
    }
}

void Debugger::Clear()
{
    meshesToRenderInFrame.clear();
}

void Debugger::StopRenderingMesh(Mesh* _mesh)
{
    for (const auto& [mesh, mat] : meshesToRender) {
        if (mesh.get() == _mesh)
        {
            meshesToRender.erase(mesh);
            return;
        }
    }
}

void Debugger::StopRenderingWidget(Widget* _widget)
{

    for (const auto& widget : widgetsToRender) {
        if (widget.get() == _widget)
        {
            erase(widgetsToRender, widget);
            return;
        }
    }
}

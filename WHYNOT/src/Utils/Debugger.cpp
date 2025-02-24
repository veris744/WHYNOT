#include "Debugger.h"

#include "Parser.h"
#include "Components/Collider.h"
#include "Graphics/Mesh.h"
#include "Components/Model.h"
#include "Graphics/Material.h"
#include "Managers/Helper.h"
#include "Managers/World.h"

bool Debugger::collisionDebugEnabled = false;
unordered_map<std::unique_ptr<Mesh>, mat4> Debugger::meshesToRender;


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

void Debugger::DrawSphereDebug(float _radius, vec3 _position, vec3 _color)
{
    vector<float> vertices;
    vector<unsigned int> indices;
    Helper::generateSphere(vertices, indices, 1, 16, 8);
    std::shared_ptr<Material> material = std::make_shared<Material>("", DEFAULT_VERTEX_SHADER_PATH, "shaders/debugFragment.glsl");
    std::unique_ptr<Mesh> sphereMesh = std::make_unique<Mesh>(vertices, vertices.size(), indices, material);
    
    sphereMesh->GetMaterial()->materialData.type = MaterialType::COLOR;
    sphereMesh->GetMaterial()->materialData.color = _color;
    mat4 mat = mat4(1.0f);
    mat = translate(mat, _position);
    mat = scale(mat, vec3(_radius));

    meshesToRender[std::move(sphereMesh)] = mat;
}

void Debugger::DrawLineDebug(vec3 _start, vec3 _end, vec3 _color)
{
    vector<float> vertices =
        {_start.x, _start.y, _start.z, 
        _end.x, _end.y, _end.z};
    std::shared_ptr<Material> material = std::make_shared<Material>("", DEFAULT_VERTEX_SHADER_PATH, "shaders/debugFragment.glsl");
    material->materialData.type = MaterialType::COLOR;
    material->materialData.color = _color;
    std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>(vertices, 2, material, 1);
    mesh->SetLinesVertexArray();
    
    mat4 mat = mat4(1.0f);
    mesh->Render(mat);

    meshesToRender[std::move(mesh)] = mat;
}

void Debugger::Render()
{
    for (const auto& [mesh, mat] : meshesToRender)
    {
        mesh->Render(mat);
    }
}

void Debugger::Clear()
{
    meshesToRender.clear();
}

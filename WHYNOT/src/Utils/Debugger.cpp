#include "Debugger.h"

#include "Graphics/Mesh.h"
#include "Components/Model.h"
#include "Graphics/Material.h"
#include "Graphics/VertexArray.h"
#include "Managers/Helper.h"

bool Debugger::collisionDebugEnabled = false;
std::unique_ptr<Mesh> Debugger::mesh = nullptr;

void Debugger::SetSphereMesh()
{
    vector<float> vertices;
    vector<unsigned int> indices;
    Helper::generateSphere(vertices, indices, 1, 16, 8);
    std::shared_ptr<Material> material = std::make_shared<Material>("", DEFAULT_VERTEX_SHADER_PATH, "shaders/debugFragment.glsl");
    mesh = std::make_unique<Mesh>(vertices, indices, material);
}

void Debugger::DrawSphereDebug(float _radius, vec3 _position, vec3 _color)
{
    if (mesh == nullptr)
    {
        SetSphereMesh();
    }
    
    mesh->GetMaterial()->materialData.type = MaterialType::COLOR;
    mesh->GetMaterial()->materialData.color = _color;
    mat4 mat = mat4(1.0f);
    mat = translate(mat, _position);
    mat = scale(mat, vec3(_radius));
    mesh->Render(mat);
}

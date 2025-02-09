#include "Debugger.h"

#include "Graphics/Mesh.h"
#include "Components/Model.h"
#include "Graphics/Material.h"
#include "Graphics/VertexArray.h"

bool Debugger::collisionDebugEnabled = false;
std::shared_ptr<Mesh> Debugger::mesh = nullptr;

void Debugger::SetSphereMesh()
{
    vector<float> vertices;
    vector<unsigned int> indices;
    Helper::generateSphere(vertices, indices, 1, 16, 8);

    std::shared_ptr<VertexArray> vertexArray = std::make_shared<VertexArray>();
    vertexArray->AddVertexBuffer(vertices, vertices.size() * sizeof(float));
    vertexArray->SetIndexBuffer(indices);
    vertexArray->Bind();
    std::shared_ptr<Material> material = std::make_shared<Material>("",
        "shaders/vertex.glsl", "shaders/debugFragment.glsl");
    mesh = std::make_shared<Mesh>(vertices, indices, material);
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

#pragma once
#include "Component.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "Transform.h"
#include "Graphics/Mesh.h"
#include "Reflection/Reflection.h"

class Material;
class Texture;
class Mesh;

class Model : public Component
{
    std::vector<std::unique_ptr<Mesh>> meshes;
    std::string directory;

    void processNode(aiNode *node, const aiScene *scene, std::shared_ptr<Material> material);
    std::unique_ptr<Mesh> processMesh(aiMesh *mesh, const aiScene *scene, std::shared_ptr<Material> material);
    std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial *_mat, aiTextureType _type, 
                                         std::string _typeName);

    Transform* transform = nullptr;
    
public:
    std::string path;
    glm::vec3 position = {0,0,0};
    glm::vec3 rotation = {0,0,0};
    glm::vec3 scale = glm::vec3(1,1,1);
    bool invertTexture = false;
    bool enableCulling = true;
    bool hasTransparency = false;
    
    Model();
    Model (const char* _path, std::shared_ptr<Material> material = nullptr)
        : path(_path)
    {
        name = "MODEL";
        if (material)
            LoadModel(path, material);
        else
            LoadModel(path);
    }
    ~Model() override = default;

    void LoadModel(std::string _path, std::shared_ptr<Material> material = nullptr);
    void AddMesh(std::unique_ptr<Mesh> _mesh);
    
    void Render();
    void Update(float deltaTime) override;
    void Clear();

    bool HasMeshes() const { return !meshes.empty(); }
    const std::vector<std::unique_ptr<Mesh>>& GetMeshes() { return meshes; }
};
REGISTER_CLASS(Model,
    REGISTER_MEMBER(Model, name, MemberProperty::Serializable),
    REGISTER_MEMBER(Model, path, MemberProperty::Viewable | MemberProperty::Serializable),
    REGISTER_MEMBER(Model, position, MemberProperty::Viewable | MemberProperty::Editable),
    REGISTER_MEMBER(Model, rotation, MemberProperty::Viewable | MemberProperty::Editable),
    REGISTER_MEMBER(Model, scale, MemberProperty::Viewable | MemberProperty::Editable),
    REGISTER_MEMBER(Model, invertTexture, MemberProperty::Viewable),
    REGISTER_MEMBER(Model, enableCulling, MemberProperty::Viewable)
);
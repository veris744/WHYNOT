#pragma once
#include "Component.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "Graphics/Mesh.h"
#include "Reflection/Reflection.h"

class Material;
class Texture;
class Mesh;

class Model : public Component
{
    vector<std::shared_ptr<Mesh>> meshes;
    string directory;

    void processNode(aiNode *node, const aiScene *scene, const std::shared_ptr<Material>& material);
    std::shared_ptr<Mesh> processMesh(aiMesh *mesh, const aiScene *scene, const std::shared_ptr<Material>& material);
    vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial *_mat, aiTextureType _type, 
                                         string _typeName);
    
public:
    string path;
    vec3 position = vec3(0,0,0);
    bool invertTexture = false;
    bool enableCulling = true;
    
    Model();
    Model (const char* _path)
        : path(_path)
    {
        name = "MODEL";
        LoadModel(path);
    }
    Model (const char* _path, const std::shared_ptr<Material>& material)
        : path(_path)
    {
        name = "MODEL";
        LoadModel(path, material);
    }
    ~Model() override = default;

    void LoadModel(string _path, const std::shared_ptr<Material>& material = nullptr);
    void AddMesh(const std::shared_ptr<Mesh>& _mesh);
    
    void Render();
    void Update(float deltaTime) override;
    void Clear();

    bool HasMeshes() { return !meshes.empty(); }
    const vector<std::shared_ptr<Mesh>>& GetMeshes() { return meshes; }
};
REGISTER_CLASS(Model, {
    REGISTER_MEMBER(Model, path),
    REGISTER_MEMBER(Model, position),
    REGISTER_MEMBER(Model, invertTexture),
    REGISTER_MEMBER(Model, enableCulling)
});
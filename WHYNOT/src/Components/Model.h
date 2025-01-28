#pragma once
#include "Component.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "Mesh.h"

class Material;
class Texture;
class Mesh;

class Model : public Component
{
    vector<std::shared_ptr<Mesh>> meshes;
    string directory;

    void LoadModel(string _path, const std::shared_ptr<Material>& material = nullptr);
    void processNode(aiNode *node, const aiScene *scene, const std::shared_ptr<Material>& material);
    std::shared_ptr<Mesh> processMesh(aiMesh *mesh, const aiScene *scene, const std::shared_ptr<Material>& material);
    vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial *_mat, aiTextureType _type, 
                                         string _typeName);
    
public:
    Model(std::shared_ptr<Mesh> _mesh);
    Model (const char* path)
    {
        name = "MODEL";
        LoadModel(path);
    }
    Model (const char* path, const std::shared_ptr<Material>& material)
    {
        name = "MODEL";
        LoadModel(path, material);
    }
    ~Model() override = default;

    vec3 position = vec3(0,0,0);
    bool invertTexture = false;
    
    void Render();
    void Update(float deltaTime) override;
    void Clear();

    bool HasMeshes() { return !meshes.empty(); }
    const vector<std::shared_ptr<Mesh>>& GetMeshes() { return meshes; }
};

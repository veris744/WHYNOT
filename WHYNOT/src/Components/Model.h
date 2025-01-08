#pragma once
#include "Component.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

class Texture;
class Mesh;

class Model : public Component
{
    vector<std::shared_ptr<Mesh>> meshes;
    string directory;

    void LoadModel(string _path);
    void processNode(aiNode *node, const aiScene *scene);
    std::shared_ptr<Mesh> processMesh(aiMesh *mesh, const aiScene *scene);
    vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial *_mat, aiTextureType _type, 
                                         string _typeName);
    
public:
    Model(std::shared_ptr<Mesh> _mesh);
    Model (const char* path)
    {
        name = "MODEL";
        LoadModel(path);
    }
    void Render();
    void Update(float deltaTime);
    void Clear();

    bool HasMeshes() { return !meshes.empty(); }
};

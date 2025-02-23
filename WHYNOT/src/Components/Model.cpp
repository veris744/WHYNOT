#include "Model.h"

#include "Graphics/Mesh.h"
#include "Transform.h"
#include "Entities/Entity.h"
#include "Graphics/Material.h"
#include "Graphics/Texture.h"
#include <assimp/postprocess.h>

#include "Managers/Renderer.h"

class Transform;

void Model::LoadModel(string _path, std::shared_ptr<Material> material)
{
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(_path.c_str(), aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType );	
	
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        Logger::Log<Model>(LogLevel::Error, import.GetErrorString());
        return;
    }
    directory = _path.substr(0, _path.find_last_of('/'));

    processNode(scene->mRootNode, scene, material);
}

void Model::processNode(aiNode* node, const aiScene* scene, std::shared_ptr<Material> material)
{
    // process all the node's meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        std::unique_ptr<Mesh> mesh_ptr = processMesh(mesh, scene, material);
        meshes.push_back(std::move(mesh_ptr));			
    }
    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene, material);
    }
}

std::unique_ptr<Mesh> Model::processMesh(aiMesh* mesh, const aiScene* scene, std::shared_ptr<Material> _material)
{
    vector<float> vertices;
    vector<unsigned int> indices;
    vector<std::shared_ptr<Texture>> textures;

    int numVertices = mesh->mNumVertices;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        // process vertex positions, normals and texture coordinates
        vertices.push_back(mesh->mVertices[i].x);
        vertices.push_back(mesh->mVertices[i].y);
        vertices.push_back(mesh->mVertices[i].z);
        
        vertices.push_back(mesh->mNormals[i].x);
        vertices.push_back(mesh->mNormals[i].y);
        vertices.push_back(mesh->mNormals[i].z);

        if (mesh->mTextureCoords[0])
        {
            vertices.push_back(mesh->mTextureCoords[0][i].x);
            vertices.push_back(mesh->mTextureCoords[0][i].y);
        }
        else
        {
            vertices.push_back(0);
            vertices.push_back(0);
        }
    }
    // process indices
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    
    // process material
    if(!_material && mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        
        vector<std::shared_ptr<Texture>> diffuseMaps = loadMaterialTextures(material, 
                                            aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        
        vector<std::shared_ptr<Texture>> specularMaps = loadMaterialTextures(material, 
                                            aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }
    std::shared_ptr<Material> material = nullptr;
    if(!_material)
    {
        material = std::make_shared<Material>(textures);
    }
    
    std::unique_ptr<Mesh> temp = std::make_unique<Mesh>(vertices, indices, _material ? _material : material);
    vertices.clear();
    return temp;
}

vector<std::shared_ptr<Texture>> Model::loadMaterialTextures(aiMaterial* _mat, aiTextureType _type, string _typeName)
{
    vector<std::shared_ptr<Texture>> textures;
    for(unsigned int i = 0; i < _mat->GetTextureCount(_type); i++)
    {
        aiString str;
        _mat->GetTexture(_type, i, &str);
        string path = directory + "/" + std::string(str.C_Str());
        
        bool skip = false;
        const vector<std::shared_ptr<Texture>>& texturesLoaded = Renderer::GetInstance()->textures_loaded;
        for (const auto& j : texturesLoaded)
        {
            if(j->GetPath() == path)
            {
                textures.push_back(j);
                skip = true; 
                break;
            }
        }

        if (!skip)
        {
            std::shared_ptr<Texture> texture = std::make_shared<Texture>(path);
            textures.push_back(texture);
            Renderer::GetInstance()->textures_loaded.push_back(texture);
        }
    }
    return textures;
}

Model::Model()
{
    name = "MODEL";
}

void Model::AddMesh(std::unique_ptr<Mesh> _mesh)
{
    meshes.push_back(std::move(_mesh));
}

void Model::Render()
{
    if (!transform)
    {
        transform = parent->GetComponent<Transform>();
    }
    for (const auto& mesh : meshes)
    {
        mat4 modelMatrix = transform->GetModelMatrix(position, rotation, scale);
        
        mesh->Render(modelMatrix);
    }
}

void Model::Update(float deltaTime)
{
}

void Model::Clear()
{
    for (const auto& mesh : meshes)
    {
        mesh->Clear();
    }
}

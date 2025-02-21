#pragma once
#include <includes.h>

#include "Reflection/Reflection.h"

#define DEFAULT_VERTEX_SHADER_PATH "shaders/vertex.glsl"
#define DEFAULT_FRAGMENT_SHADER_PATH "shaders/fragment.glsl"

class Shader;
class Texture;

enum class MaterialType
{
    TEXTURE, COLOR, NEON
};
REGISTER_ENUM(MaterialType,
    {"TEXTURE", MaterialType::TEXTURE},
    {"COLOR", MaterialType::COLOR},
    {"NEON", MaterialType::NEON},
);

struct MaterialData
{
    MaterialType type = MaterialType::TEXTURE;
    int ambient = 0;
    int diffuse = 0;
    int specular = 0;
    float shininess = 32;
    vec3 color = vec3(1, 1, 1);
};
REGISTER_CLASS(MaterialData, {
    REGISTER_MEMBER(MaterialData, type),
    REGISTER_MEMBER(MaterialData, diffuse),
    REGISTER_MEMBER(MaterialData, specular),
    REGISTER_MEMBER(MaterialData, shininess),
    REGISTER_MEMBER(MaterialData, color),
});

class Material
{
    vector<std::shared_ptr<Texture>> textures;
    std::shared_ptr<Shader> shader;
    
public:
    Material(const char* _texturePath, const string& _vertexShaderPath = DEFAULT_VERTEX_SHADER_PATH,
        const string& _fragmentShaderPath = DEFAULT_FRAGMENT_SHADER_PATH, MaterialData _materialData = MaterialData());
    Material(const vector<string>& _texturePaths, const string& _vertexShaderPath = DEFAULT_VERTEX_SHADER_PATH,
        const string& _fragmentShaderPath = DEFAULT_FRAGMENT_SHADER_PATH, MaterialData _materialData = MaterialData());
    Material(const vector<std::shared_ptr<Texture>>& _texures, const string& _vertexShaderPath = DEFAULT_VERTEX_SHADER_PATH,
        const string& _fragmentShaderPath = DEFAULT_FRAGMENT_SHADER_PATH, MaterialData _materialData = MaterialData());
    Material() = default;
    ~Material() = default;

    MaterialData materialData;
    string vertexShaderPath = DEFAULT_VERTEX_SHADER_PATH;
    string fragmentShaderPath = DEFAULT_FRAGMENT_SHADER_PATH;
    vector<string> texturePaths;

    void InitializeShader();
    void AddTexture(const std::shared_ptr<Texture>& _texture);
    void SetUniforms(const mat4& _model, const mat4& _view, const mat4& _projection, const vec3& _viewPosition);
    
    void BindTexture() const;
    void UnbindTexture() const;
    
    void BindShader() const;
    void UnbindShader() const;
    
};
REGISTER_CLASS(Material, {
    REGISTER_MEMBER(Material, materialData),
    REGISTER_MEMBER(Material, vertexShaderPath),
    REGISTER_MEMBER(Material, fragmentShaderPath),
    REGISTER_MEMBER(Material, texturePaths)
});
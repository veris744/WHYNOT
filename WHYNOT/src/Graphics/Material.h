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

struct MaterialData : ReflectedObject
{
    MaterialType type = MaterialType::TEXTURE;
    int ambient = 0;
    int diffuse = 0;
    int specular = 0;
    float shininess = 32;
    vec4 color = vec4(1, 1, 1, 1);
};
REGISTER_CLASS(MaterialData, {
    REGISTER_MEMBER(MaterialData, type, MemberProperty::NonViewable),
    REGISTER_MEMBER(MaterialData, diffuse, MemberProperty::Viewable),
    REGISTER_MEMBER(MaterialData, specular, MemberProperty::Viewable),
    REGISTER_MEMBER(MaterialData, shininess, MemberProperty::Viewable),
    REGISTER_MEMBER(MaterialData, color, MemberProperty::Viewable),
});

class Material : public ReflectedObject
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

    Shader* GetShader() const { return shader.get(); };
    const vector<std::shared_ptr<Texture>>& GetTexture() const { return textures; }; ;
    
};
REGISTER_CLASS(Material, {
    REGISTER_MEMBER(Material, materialData, MemberProperty::NonViewable),
    REGISTER_MEMBER(Material, vertexShaderPath, MemberProperty::Viewable),
    REGISTER_MEMBER(Material, fragmentShaderPath, MemberProperty::Viewable),
    REGISTER_MEMBER(Material, texturePaths, MemberProperty::Hidden)
});
#pragma once

#include "Texture.h"
#include "Reflection/Reflection.h"

#define DEFAULT_VERTEX_SHADER_PATH "shaders/vertex.glsl"
#define DEFAULT_FRAGMENT_SHADER_PATH "shaders/fragment.glsl"

class Shader;

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
    glm::vec4 color = glm::vec4(1, 1, 1, 1);
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
    std::vector<std::shared_ptr<Texture>> textures;
    std::shared_ptr<Shader> shader;
    
public:
    Material(const char* _texturePath, const std::string& _vertexShaderPath = DEFAULT_VERTEX_SHADER_PATH,
        const std::string& _fragmentShaderPath = DEFAULT_FRAGMENT_SHADER_PATH, MaterialData _materialData = MaterialData());
    Material(const std::vector<std::string>& _texturePaths, const std::string& _vertexShaderPath = DEFAULT_VERTEX_SHADER_PATH,
        const std::string& _fragmentShaderPath = DEFAULT_FRAGMENT_SHADER_PATH, MaterialData _materialData = MaterialData());
    Material(const std::vector<std::shared_ptr<Texture>>& _texures, const std::string& _vertexShaderPath = DEFAULT_VERTEX_SHADER_PATH,
        const std::string& _fragmentShaderPath = DEFAULT_FRAGMENT_SHADER_PATH, MaterialData _materialData = MaterialData());
    Material() = default;
    ~Material() = default;

    MaterialData materialData;
    std::string vertexShaderPath = DEFAULT_VERTEX_SHADER_PATH;
    std::string fragmentShaderPath = DEFAULT_FRAGMENT_SHADER_PATH;
    std::vector<std::string> texturePaths;
    WrapMode wrapMode = WrapMode::CLAMP_TO_EDGE;
    FilterMode filterMode = FilterMode::LINEAR;
    MipmapMode mipmapMode = MipmapMode::LINEAR_MIPMAP_LINEAR;

    void InitializeShader();
    void AddTexture(const std::shared_ptr<Texture>& _texture);
    void SetUniforms(const glm::mat4& _model, const glm::mat4& _view, const glm::mat4& _projection, const glm::vec3& _viewPosition);
    
    void BindTexture() const;
    void UnbindTexture() const;
    
    void BindShader() const;
    void UnbindShader() const;

    Shader* GetShader() const { return shader.get(); };
    const std::vector<std::shared_ptr<Texture>>& GetTexture() const { return textures; }; ;
    
};
REGISTER_CLASS(Material, {
    REGISTER_MEMBER(Material, texturePaths, MemberProperty::NonViewable),
    REGISTER_MEMBER(Material, vertexShaderPath, MemberProperty::Viewable),
    REGISTER_MEMBER(Material, fragmentShaderPath, MemberProperty::Viewable),
    REGISTER_MEMBER(Material, materialData, MemberProperty::NonViewable),
    REGISTER_MEMBER(Material, wrapMode, MemberProperty::Viewable),
    REGISTER_MEMBER(Material, filterMode, MemberProperty::Viewable),
    REGISTER_MEMBER(Material, mipmapMode, MemberProperty::Viewable),
});
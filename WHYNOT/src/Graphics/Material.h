#pragma once
#include <includes.h>

#include "MaterialData.h"
#define DEFAULT_VERTEX_SHADER_PATH "shaders/vertex.glsl"
#define DEFAULT_FRAGMENT_SHADER_PATH "shaders/fragment.glsl"

class Shader;
class Texture;

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

    MaterialData materialData;
    
    void SetUniforms(const mat4& _model, const mat4& _view, const mat4& _projection, const vec3& _viewPosition);
    
    void BindTexture() const;
    void UnbindTexture() const;
    
    void BindShader() const;
    void UnbindShader() const;
    
};

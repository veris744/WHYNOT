#pragma once
#include <glad/include/glad/glad.h>
#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

using namespace std;
using namespace glm;

class Shader
{
    unordered_map<int, GLuint> uboMap;
    
public:
    string vertexShaderPath;
    string fragmentShaderPath;
    string vertexShaderSource;
    string fragmentShaderSource;
    
    int id;
    
    Shader(const string& _vertexShaderPath, const string& _fragmentShaderPath);
    ~Shader()
    {
        glDeleteProgram(id);
    }
    string ReadShader(const string& ShaderPath);

    void Compile();
    void Bind() const;
    void Unbind() const;
    void CleanUp();

    void SetUniformBool(const char* name, bool value) const;
    void SetUniformInt(const char* name, int value) const;
    void SetUniformFloat(const char* name, float value) const;
    void SetUniformVec2(const char* name, const vec2& value) const;
    void SetUniformVec3(const char* name, const vec3& value) const;
    void SetUniformVec4(const char* name, const vec4& value) const;
    void SetUniformMat4(const char* name, const mat4& value) const;
    void SetUniformObject(const char* name, int bindingPoint, int maxObjects, int sizeObject, void* objects, bool _static = true);
};

#pragma once
#include <glad/include/glad/glad.h>
#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

class Shader
{
    std::unordered_map<int, GLuint> uboMap;
    
public:
    std::string vertexShaderPath;
    std::string fragmentShaderPath;
    std::string vertexShaderSource;
    std::string fragmentShaderSource;
    
    int id;
    
    Shader(const std::string& _vertexShaderPath, const std::string& _fragmentShaderPath);
    ~Shader()
    {
        glDeleteProgram(id);
    }
    std::string ReadShader(const std::string& ShaderPath);

    void Compile();
    void Bind() const;
    void Unbind() const;
    void CleanUp();

    void SetUniformBool(const char* name, bool value) const;
    void SetUniformInt(const char* name, int value) const;
    void SetUniformFloat(const char* name, float value) const;
    void SetUniformVec2(const char* name, const glm::vec2& value) const;
    void SetUniformVec3(const char* name, const glm::vec3& value) const;
    void SetUniformVec4(const char* name, const glm::vec4& value) const;
    void SetUniformMat4(const char* name, const glm::mat4& value) const;
    void SetUniformObject(const char* name, int bindingPoint, int maxObjects, int sizeObject, void* objects, bool _static = true);
};

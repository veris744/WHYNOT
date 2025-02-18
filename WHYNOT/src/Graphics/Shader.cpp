#include "Shader.h"

#include "Managers/Renderer.h"

Shader::Shader(const string& _vertexShaderPath, const string& _fragmentShaderPath)
{
    vertexShaderPath = _vertexShaderPath;
    fragmentShaderPath = _fragmentShaderPath;
    vertexShaderSource = ReadShader(vertexShaderPath);
    fragmentShaderSource = ReadShader(fragmentShaderPath);
}

string Shader::ReadShader(const string& vertexShaderPath)
{
    std::ifstream istream(vertexShaderPath.c_str(), std::ios_base::binary);
    std::stringstream sstream;
    sstream << istream.rdbuf();
    string temp = sstream.str();
    return temp;
}

void Shader::Compile()
{
    const char* _vertexShaderSource = vertexShaderSource.c_str();
    uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &_vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    
    GLint retCode;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &retCode);
    if (retCode == GL_FALSE) {
        char errorLog[512];
        glGetShaderInfoLog(vertexShader, sizeof(errorLog), nullptr, errorLog);
        Logger::Log<Shader>(LogLevel::FatalError, "Error compiling vertex shader " + vertexShaderPath + ": " + string(errorLog));
        glDeleteShader(vertexShader);
    }

    const char* _fragmentShaderSource = fragmentShaderSource.c_str();
    uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &_fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &retCode);
    if (retCode == GL_FALSE) {
        char errorLog[1024];
        glGetShaderInfoLog(fragmentShader, sizeof(errorLog), nullptr, errorLog);
        Logger::Log<Shader>(LogLevel::FatalError, "Error compiling fragment shader " + fragmentShaderPath + ": " + string(errorLog));
        glDeleteShader(fragmentShader);
    }

    id = glCreateProgram();

    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glLinkProgram(id);
    
    glGetProgramiv(id, GL_LINK_STATUS, &retCode);
    if (retCode == GL_FALSE) {
        char errorLog[512];
        glGetProgramInfoLog(id, sizeof(errorLog), nullptr, errorLog);
        std::cout << "Error compiling: " << errorLog << '\n';
        glDeleteProgram(id);
        id = 0;
        std::exit(EXIT_FAILURE);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::Bind() const
{
    glUseProgram(id);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

void Shader::CleanUp()
{        
    for (auto& pair : uboMap) {
        glDeleteBuffers(1, &pair.second);
    }
    uboMap.clear();
}

void Shader::SetUniformBool(const char* name, bool value) const
{
    glUniform1i(glGetUniformLocation(id, name), value);
}

void Shader::SetUniformInt(const char* name, int value) const
{
    glUniform1i(glGetUniformLocation(id, name), value);
}

void Shader::SetUniformFloat(const char* name, float value) const
{
    glUniform1f(glGetUniformLocation(id, name), value);
}

void Shader::SetUniformVec2(const char* name, const vec2& value) const
{
    glUniform2fv(glGetUniformLocation(id, name), 1, glm::value_ptr(value));
}

void Shader::SetUniformVec3(const char* name, const vec3& value) const
{
    glUniform3fv(glGetUniformLocation(id, name), 1, glm::value_ptr(value));
}

void Shader::SetUniformVec4(const char* name, const vec4& value) const
{
    glUniform4fv(glGetUniformLocation(id, name), 1, glm::value_ptr(value));
}

void Shader::SetUniformMat4(const char* name, const mat4& value) const
{
    glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetUniformObject(const char* name, int bindingPoint, int maxObjects, int sizeObject, void* objects, bool _static)
{
    GLuint uboObjects;
    
    // Check if UBO for this binding point already exists
    if (uboMap.find(bindingPoint) == uboMap.end()) {
        // Create and store a new UBO
        glGenBuffers(1, &uboObjects);
        uboMap[bindingPoint] = uboObjects;
        glBindBuffer(GL_UNIFORM_BUFFER, uboObjects);
        glBufferData(GL_UNIFORM_BUFFER, maxObjects * sizeObject, nullptr, _static ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
    } else
    {
        // Reuse existing UBO
        uboObjects = uboMap[bindingPoint];
        
        // Validate the UBO
        if (glIsBuffer(uboObjects) == GL_FALSE) {
            Logger::Log(LogLevel::Warning,  "Warning: UBO " + to_string(uboObjects) + " is invalid. Recreating.\n");
            glGenBuffers(1, &uboObjects);
            uboMap[bindingPoint] = uboObjects;
            glBindBuffer(GL_UNIFORM_BUFFER, uboObjects);
            glBufferData(GL_UNIFORM_BUFFER, maxObjects * sizeObject, nullptr, _static ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
        } else {
            glBindBuffer(GL_UNIFORM_BUFFER, uboObjects);
        }
    }

    // Update UBO contents
    if (objects != nullptr) {
        glBufferSubData(GL_UNIFORM_BUFFER, 0, maxObjects * sizeObject, objects);
    }
    
    // Bind the UBO to the specified binding point
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, uboObjects);

    // Unbind the UBO to prevent unintended modifications
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // Optional: Debug validation
    GLint maxUBOSize;
    glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxUBOSize);
    if (maxObjects * sizeObject > maxUBOSize) {
        std::cerr << "Error: UBO size exceeds hardware limits!" << std::endl;
        return;
    }
}


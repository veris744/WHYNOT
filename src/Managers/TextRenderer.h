#pragma once
#include <glad/include/glad/glad.h>
#include <ft2build.h>
#include <map>
#include <memory>
#include <string>
#include <glm/glm.hpp>
#include <Graphics/Shader.h>

#include FT_FREETYPE_H

class VertexArray;

using namespace std;
using namespace glm;

struct Character {
    unsigned int TextureID; 
    ivec2   Size;
    ivec2   Bearing;
    unsigned int Advance;
};

class TextRenderer
{
    string fontPathDefault = "assets/fonts/default.ttf";
    static unique_ptr<FT_Library> ftLibrary;
    map<GLchar, Character> Characters;

    unsigned int VAO, VBO;
    string defaultVertexShaderPath = "shaders/textVertexShader.glsl";
    string defaultFragmentShaderPath = "shaders/textFragmentShader.glsl";
    std::unique_ptr<Shader> defaultShader;
    
public:
    TextRenderer() = default;
    static TextRenderer& instance() {
        static TextRenderer INSTANCE;
        return INSTANCE;
    }

    static void InitFreeType();
    void LoadFont(const string& _fontPath = "");
    
    void RenderText(string text, float x, float y, float scale, vec3 color, unsigned int layer);
    void Clear();

    const std::map<GLchar, Character>& GetCharacters() const { return Characters; };
    
};

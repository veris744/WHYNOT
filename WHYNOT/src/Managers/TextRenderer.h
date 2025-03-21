#pragma once
#include <includes.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Graphics/Shader.h"

class VertexArray;

struct Character {
    unsigned int TextureID; 
    glm::ivec2   Size;
    glm::ivec2   Bearing;
    unsigned int Advance;
};

class TextRenderer
{
    string fontPathDefault = "assets/fonts/default.ttf";
    static std::unique_ptr<FT_Library> ftLibrary;
    std::map<GLchar, Character> Characters;

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

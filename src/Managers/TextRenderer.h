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

struct Character {
    unsigned int TextureID;
    glm::ivec2   Size;
    glm::ivec2   Bearing;
    unsigned int Advance;
};

class TextRenderer
{
    std::string fontPathDefault = "assets/fonts/default.ttf";
    static std::unique_ptr<FT_Library> ftLibrary;
    std::map<GLchar, Character> Characters;

    unsigned int VAO, VBO;
    std::string defaultVertexShaderPath = "shaders/textVertexShader.glsl";
    std::string defaultFragmentShaderPath = "shaders/textFragmentShader.glsl";
    std::unique_ptr<Shader> defaultShader;
    
public:
    TextRenderer() = default;
    static TextRenderer& instance() {
        static TextRenderer INSTANCE;
        return INSTANCE;
    }

    static void InitFreeType();
    void LoadFont(const std::string& _fontPath = "");
    
    void RenderText(std::string text, float x, float y, float scale, glm::vec3 color, unsigned int layer);
    void Clear();

    const std::map<GLchar, Character>& GetCharacters() const { return Characters; };
    
};

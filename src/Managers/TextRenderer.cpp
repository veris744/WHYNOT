#include "TextRenderer.h"

#include <glm/ext/matrix_clip_space.hpp>

#include "Helper.h"
#include "World.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"

std::unique_ptr<FT_Library> TextRenderer::ftLibrary = nullptr;

void TextRenderer::InitFreeType()
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        Logger::Log<TextRenderer>(LogLevel::FatalError, "ERROR::FREETYPE: Could not init FreeType Library");
    }
    ftLibrary = std::make_unique<FT_Library>(ft);
}

void TextRenderer::LoadFont(const string& _fontPath)
{
    defaultShader = std::make_unique<Shader>(defaultVertexShaderPath, defaultFragmentShaderPath);
    defaultShader->Compile();
    
    FT_Face face;
    if (FT_New_Face(*ftLibrary, _fontPath.empty() ? fontPathDefault.c_str() : _fontPath.c_str(), 0, &face)) {
        Logger::Log<TextRenderer>(LogLevel::FatalError, "Failed to load font");
    }
    else
    {
        FT_Set_Pixel_Sizes(face, 0, 48);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
        for (unsigned char c = 0; c < 128; c++)
        {
            // Load character glyph 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                Logger::Log<TextRenderer>(LogLevel::Error, "ERROR::FREETYPE: Failed to load Glyph");
                continue;
            }
            if (!face->glyph->bitmap.buffer)
            {
                continue;
            }
            // generate texture
            std::unique_ptr<Texture> texture = std::make_unique<Texture>(face->glyph->bitmap.buffer,
                face->glyph->bitmap.rows * face->glyph->bitmap.pitch,
                face->glyph->bitmap.width, face->glyph->bitmap.rows, 1);

            Character character = {
                texture->id,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
            };
            Characters.insert(std::pair<char, Character>(c, character));
        }
    }
    FT_Done_Face(face);
    FT_Done_FreeType(*ftLibrary);

    // configure VAO/VBO for texture quads
    // -----------------------------------
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void TextRenderer::RenderText(string text, float x, float y, float scale, vec3 color, unsigned int layer)
{
    defaultShader->Bind();
    defaultShader->SetUniformVec3("textColor", color);
    mat4 projection = ortho(0.0f, Helper::windowWidth, Helper::windowHeight, 0.0f);
    defaultShader->SetUniformMat4("projection", projection);
    defaultShader->SetUniformFloat("layer", 0.1f * layer);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // iterate through all characters
    std::string::const_iterator c;
    for (char c : text)
    {
        if (c == ' ') {
            // Ensure space character exists in Characters map
            if (Characters.find(' ') != Characters.end()) {
                x += (Characters[' '].Advance >> 6) * scale;
            } else {
                // Fallback space width if not defined (typically about 1/3 of 'n' width)
                float spaceWidth = (Characters['n'].Advance >> 6) * scale * 0.33f;
                x += spaceWidth;
            }
            continue;
        }
        Character ch = Characters[c];

        float xpos = x + ch.Bearing.x * scale;
        // Changed this line to properly handle baseline alignment
        float ypos = y + (Characters['H'].Bearing.y - ch.Bearing.y) * scale;

        // Special adjustments for punctuation
        if (c == '.' || c == ',')
        {
            ypos += 2.0f * scale; // smaller adjustment for better alignment
        }
        else if (c == 'g' || c == 'j' || c == 'p' || c == 'q' || c == 'y')
        {
            ypos -= 2.0f * scale; // slight adjustment for descenders
        }

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 1.0f },
            { xpos,     ypos,       0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 0.0f },

            { xpos,     ypos + h,   0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 0.0f },
            { xpos + w, ypos + h,   1.0f, 1.0f }
        };

        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += (ch.Advance >> 6) * scale;
    }
}

void TextRenderer::Clear()
{
    defaultShader->Unbind();
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

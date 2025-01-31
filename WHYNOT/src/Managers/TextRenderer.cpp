#include "TextRenderer.h"

#include "World.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"

std::shared_ptr<TextRenderer> TextRenderer::instance = nullptr;
std::unique_ptr<FT_Library> TextRenderer::ftLibrary = nullptr;

TextRenderer::TextRenderer()
= default;

std::shared_ptr<TextRenderer> TextRenderer::GetInstance()
{
    if (!instance)
    {
        instance = std::make_shared<TextRenderer>();
    }
    return instance;
}

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
        Logger::Log<TextRenderer>(LogLevel::FatalError, "ERROR::FREETYPE: Failed to load font");
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

void TextRenderer::RenderText(string text, float x, float y, float scale, vec3 color)
{
    defaultShader->Bind();
    defaultShader->SetUniformVec3("textColor", color);
    mat4 projection = glm::ortho(0.0f, Helper::windowWidth, 0.0f, Helper::windowHeight);
    defaultShader->SetUniformMat4("projection", projection);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) 
    {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },            
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }           
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
}

void TextRenderer::Clear()
{
    defaultShader->Unbind();
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

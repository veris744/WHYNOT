#include "Texture.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Managers/Renderer.h"


Texture::Texture(const string& filePath)
{
    path = filePath;
    LoadTexture(filePath);
    ProcessTexture();
}

void Texture::LoadTexture(const string& filePath)
{
    int width;
    int height;
    data = stbi_load(filePath.c_str(), &width, &height, &nbChannels, 0);
    size = vec2(width, height);
    if (!data)
    {
        Logger::Log<Texture>(LogLevel::Error, "Failed to load texture at path: " + filePath);
    }
}

void Texture::ProcessTexture()
{
    glGenTextures(1, &id);
    Bind();
    SetParameters();
    Generate();
    Renderer::GetInstance()->textures_loaded.push_back(std::make_shared<Texture>(*this));
}

void Texture::SetParameters() const
{
    auto wrap = GL_REPEAT;
    switch (wrapMode)
    {
        case WrapMode::REPEAT:
            wrap = GL_REPEAT;
            break;
        case WrapMode::MIRRORED_REPEAT:
            wrap = GL_MIRRORED_REPEAT;
            break;
        case WrapMode::CLAMP_TO_EDGE:
            wrap = GL_CLAMP_TO_EDGE;
            break;
        case WrapMode::CLAMP_TO_BORDER:
            wrap = GL_CLAMP_TO_BORDER;
            break;
    }
    auto filter = GL_LINEAR;
    switch (filterMode)
    {
        case FilterMode::NEAREST:
            filter = GL_NEAREST;
            break;
        case FilterMode::LINEAR:
            filter = GL_LINEAR;
            break;
    }
    auto mipmap = GL_LINEAR_MIPMAP_LINEAR;
    switch (mipmapMode)
    {
        case MipmapMode::LINEAR_MIPMAP_LINEAR:
            mipmap = GL_LINEAR;
            break;
        case MipmapMode::NEAREST_MIPMAP_NEAREST:
            mipmap = GL_NEAREST;
            break;
        case MipmapMode::LINEAR_MIPMAP_NEAREST:
            mipmap = GL_LINEAR;
            break;
        case MipmapMode::NEAREST_MIPMAP_LINEAR:
            mipmap = GL_NEAREST;
            break;
    }
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    
}

void Texture::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::Activate(unsigned int _textureNb) const
{
    glActiveTexture(GL_TEXTURE0 + _textureNb);
}

void Texture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);  
}

void Texture::Generate() const
{
    int colorMode;
    if (nbChannels == 1)
        colorMode = GL_RED;
    else if (nbChannels == 2)
        colorMode = GL_RG;
    else if (nbChannels == 3)
        colorMode = GL_RGB;
    else
        colorMode = GL_RGBA;
    
    glTexImage2D(GL_TEXTURE_2D, 0, colorMode,size.x, size.y, 0, colorMode, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}
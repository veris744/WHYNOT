#pragma once
#include "includes.h"

enum class WrapMode
{
    REPEAT, MIRRORED_REPEAT, CLAMP_TO_EDGE, CLAMP_TO_BORDER
};

enum class FilterMode
{
    LINEAR, NEAREST
};

enum class MipmapMode
{
    NEAREST_MIPMAP_NEAREST, LINEAR_MIPMAP_LINEAR, NEAREST_MIPMAP_LINEAR, LINEAR_MIPMAP_NEAREST
};


class Texture
{
    string path;
    unsigned char *data;
    vec2 size;
    int nbChannels;
        
    public:
        Texture(const string& filePath);
        Texture(unsigned char *_data, unsigned int _bufferSize, int _width, int _height, int _channels);
        ~Texture()
        {
            if (data)
            {
                delete[] data;
                data = nullptr;
            }
        }
        Texture(const Texture& _texture)
        {
            id = _texture.id;
            path = _texture.GetPath();
            size = _texture.size;
            nbChannels = _texture.nbChannels;
            data = nullptr;
        }
    
        void LoadTexture(const string& filePath);
        void ProcessTexture();
        
        void SetParameters() const;
        void Bind() const;
        void Activate(unsigned int _textureNb) const;
        void Unbind();
        void Generate();
        
        WrapMode wrapMode = WrapMode::REPEAT;
        FilterMode filterMode = FilterMode::LINEAR;
        MipmapMode mipmapMode = MipmapMode::LINEAR_MIPMAP_LINEAR;
        unsigned int id;

        const string& GetPath() const { return path; }
        int GetNbChannels() const { return nbChannels; }

        void CleanUp();
};

#pragma once

enum class MaterialType
{
    TEXTURE, COLOR, NEON
};

struct MaterialData
{
    MaterialType type = MaterialType::TEXTURE;
    int ambient = 0;
    int diffuse = 0;
    int specular = 0;
    float shininess = 32;
    vec3 color = vec3(1, 1, 1);
};

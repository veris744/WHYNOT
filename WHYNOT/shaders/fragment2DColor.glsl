#version 420 core
out vec4 FragColor;

in vec2 aUV;

uniform vec4 uColor;

void main()
{
    FragColor = uColor;
} 
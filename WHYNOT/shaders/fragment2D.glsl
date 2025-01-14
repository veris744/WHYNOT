#version 420 core
out vec4 FragColor;

in vec2 aUV;

uniform sampler2D uSampler2D;

void main()
{
    FragColor = texture(uSampler2D, aUV);
} 
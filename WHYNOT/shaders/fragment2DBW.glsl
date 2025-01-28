#version 420 core
out vec4 FragColor;

in vec2 aUV;

uniform sampler2D uSampler2D;

void main()
{
    float value = texture(uSampler2D, aUV).r;
    FragColor = vec4(0, 0, 0, value == 1 ? 0 : 1);
} 
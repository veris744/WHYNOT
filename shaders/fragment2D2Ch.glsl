#version 420 core
out vec4 FragColor;

in vec2 aUV;

uniform sampler2D uSampler2D;
uniform vec3 uColor;

void main()
{
    float value = texture(uSampler2D, aUV).g;
    FragColor = vec4(uColor, value);
} 
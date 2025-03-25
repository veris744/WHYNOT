#version 420 core
out vec4 FragColor;

in vec2 aUV;
in vec3 aFragPosition;
in vec3 aNormal;

uniform vec3 uViewPos;

uniform sampler2D uAmbient;


void main()
{
    FragColor    = vec4(texture(uAmbient, aUV));
    
}
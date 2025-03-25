#version 420 core
layout (location = 0) in vec3 uPos;
layout (location = 1) in vec3 uNormal;
layout (location = 2) in vec2 uUV;

//out vec3 aColor;
out vec2 aUV;
out vec3 aFragPosition;
out vec3 aNormal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;


void main()
{
    gl_Position = uProjection * uView * uModel * vec4(uPos, 1.0);
    aFragPosition = vec3(uModel * vec4(uPos, 1.0));
    aNormal = normalize(mat3(transpose(inverse(uModel))) * uNormal);
    aUV = uUV;
}
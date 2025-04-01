#version 410 core
layout (location = 0) in vec2 uPos;
layout (location = 1) in vec2 uUV;

uniform vec2 uPosWidget;
uniform vec2 uSize;
uniform float uLayer;
uniform float uRotation;

uniform mat4 uProjection;
out vec2 aUV;


void main()
{
    vec2 rotatedPos = uPos;
    if (uRotation != 0)
    {
        float rad = radians(uRotation);
        mat2 rotationMatrix = mat2(cos(rad), -sin(rad), sin(rad), cos(rad));
        rotatedPos = rotationMatrix * uPos;
    }

    vec2 transformedPos = rotatedPos  * uSize + uPosWidget;
    gl_Position = uProjection * vec4(transformedPos, uLayer, 1.0);
    aUV = uUV;
}
#version 420
out vec4 FragColor;

uniform vec3 uColor;

void main() {
    FragColor = vec4(uColor , 0.5);
}
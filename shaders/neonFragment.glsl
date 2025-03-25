#version 420 core
out vec4 FragColor;

in vec3 aFragPosition;
in vec3 aNormal;

uniform vec3 uViewPos;

uniform vec3 uColor;  // Neon light base color
uniform float uGlowIntensity;  // Intensity of the glow
uniform float uEdgeGlow;       // Strength of edge glow (Fresnel effect)


void main() {
    vec3 normal = normalize(aNormal);
    vec3 viewDir = normalize(uViewPos - aFragPosition);

    float fresnel = 1.0 - max(dot(normal, viewDir), 0.0);
    fresnel = pow(fresnel, 1.0);

    float glow = uGlowIntensity * fresnel;
    vec3 interiorColor = vec3(0, 0.7 , 0.5);
    vec3 color = mix(interiorColor, uColor, fresnel);

    FragColor = vec4(color, 1.0); 
}

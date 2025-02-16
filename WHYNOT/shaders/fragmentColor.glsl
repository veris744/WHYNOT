#version 420 core
out vec4 FragColor;

in vec2 aUV;
in vec3 aFragPosition;
in vec3 aNormal;

uniform vec3 uViewPos;
uniform vec3 uColor;

uniform float uShininess;

struct LightSource {
    vec3 position;
    vec3 color;
    vec3 direction;
    float specular;
    float diffuse;
    float ambient;
    float constantAtt;
    float linearAtt;
    float quadAtt;
    float angle;
    float outerAngle;
    int type;
};

uniform int uNumLights;
layout (std140, binding = 1) uniform LightsBlock {
    LightSource uLights [10];
};


void main()
{
    vec3 diffuseTotal   = vec3(0);
    vec3 specularTotal  = vec3(0);
    vec3 ambient        = vec3(0);


    for (int i = 0; i < uNumLights; i++)
    {
        if (uLights[i].type == 3)
        {
            // Ambient
            ambient = uLights[0].color * uLights[0].ambient * uColor;
            continue;
        }
        
        float f_CutOff = cos(uLights[i].angle * (3.14 / 180));
        float f_OuterCuttoff = cos(uLights[i].outerAngle * (3.14 / 180));

        float f_Attenuation = 1;
        vec3 LightDir     = vec3(0.0);

        if (uLights[i].type == 0)
        {
            LightDir = normalize(-uLights[i].direction);
        }
        else
        {
            LightDir      = normalize(uLights[i].position - aFragPosition);
            float distance  = length(uLights[i].position - aFragPosition);
            if (uLights[i].constantAtt + uLights[i].linearAtt + uLights[i].quadAtt != 0)
            {
                f_Attenuation   = 1 / (uLights[i].constantAtt + uLights[i].linearAtt * distance + uLights[i].quadAtt * (distance * distance));
            }
        }

        vec3 diffuse   = vec3(0.0);
        vec3 specular  = vec3(0.0);

        float f_DotProduct  = 0;
        float f_Epsilon     = 0;
        float f_Intensity   = 1;

        if (uLights[i].type == 2)
        {
            f_DotProduct = dot(LightDir, normalize(-uLights[i].direction));
            f_Epsilon   = f_CutOff - f_OuterCuttoff;
            f_Intensity = clamp((f_DotProduct - f_OuterCuttoff) / f_Epsilon, 0.0, 1.0);
        }
        
        if (uLights[i].type != 2 || (uLights[i].type == 2 && f_DotProduct > f_OuterCuttoff))
        {
            // Diffuse
            vec3  norm      = normalize(aNormal);
            float diff      = max(dot(norm, LightDir), 0.0);
            diffuse         = diff * uLights[i].color * uLights[i].diffuse * uColor * f_Attenuation * f_Intensity;
            
            // Specular
            vec3 viewDir    = normalize(uViewPos - aFragPosition);
            vec3 reflectDir = reflect(-LightDir, aNormal);
            float spec      = pow(max(dot(viewDir, reflectDir), 0.0), uShininess);
            specular        = uLights[i].color * uLights[i].specular * spec * uColor * f_Attenuation * f_Intensity;

        }
        diffuseTotal    = diffuseTotal + diffuse;
        specularTotal   = specularTotal + specular;
    }
    FragColor    = vec4(ambient + diffuseTotal + specularTotal, 1.f);
    
}
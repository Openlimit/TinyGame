#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec4 FragPosLightSpace;

struct DirectionLight {
    vec3 direction;
	vec3 color;
};

uniform sampler2D depthMap;

uniform DirectionLight directionLight;
uniform vec3 diffuse_color;
uniform vec3 viewPos;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    
    float closestDepth = texture(depthMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    float bias = max(0.05 * (1.0 - dot(Normal, -directionLight.direction)), 0.005);
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    return shadow;
}

vec3 CalcDirLight(DirectionLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    float shadow = ShadowCalculation(FragPosLightSpace);
    return ((1.0 - shadow) *light.color * diff +0.1)* diffuse_color;
}

void main()
{
	vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = CalcDirLight(directionLight, norm, viewDir);
    FragColor = vec4(result, 1.0);
}
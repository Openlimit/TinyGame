#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec4 FragPosLightSpace;

struct DirectionLight {
    vec3 direction;
	vec3 color;
};

struct PointLight{
    vec3 position;
	vec3 color;
};

uniform sampler2D directionShadowMap;
uniform samplerCube pointShadowMap;

uniform DirectionLight directionLight;
uniform PointLight pointLight;

uniform vec3 diffuse_color;
uniform vec3 cameraPos;
uniform float far_plane;
uniform float receiveShadow;

float PointShadowCalculation()
{
    vec3 fragToLight = FragPos - pointLight.position;  
    float closestDepth = texture(pointShadowMap, fragToLight).r;
    if(closestDepth < 1)
    {
        closestDepth *= far_plane;

        float currentDepth = length(fragToLight);
    
        float bias = 0.05; 
        float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;
        return shadow;
    }
    else
    {
        return 0;
    }
}

float DirectShadowCalculation(vec4 fragPosLightSpace)
{
    
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float currentDepth = projCoords.z;
    if(currentDepth>1)
        return 0.0;

    float closestDepth = texture(directionShadowMap, projCoords.xy).r; 
    
    float bias = max(0.05 * (1.0 - dot(Normal, -directionLight.direction)), 0.005);
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    return shadow;
}

vec3 CalcDirLight(DirectionLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    float shadow=0;
    if(receiveShadow>0)
        shadow = DirectShadowCalculation(FragPosLightSpace);

    return ((1.0 - shadow) *light.color * diff +0.1)* diffuse_color;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    float shadow=0;
    if(receiveShadow>0)
        shadow= PointShadowCalculation();

    return ((1.0 - shadow) *light.color * diff +0.1)* diffuse_color;
}

void main()
{
	vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 result = CalcDirLight(directionLight, norm, viewDir);
    //vec3 result = CalcPointLight(pointLight, norm, viewDir);
    FragColor = vec4(result, 1.0);
}
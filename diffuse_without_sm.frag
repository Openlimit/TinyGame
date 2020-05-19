#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

struct DirectionLight {
    vec3 direction;
	vec3 color;
};

uniform DirectionLight directionLight;

uniform vec3 diffuse_color;
uniform vec3 cameraPos;

vec3 CalcDirLight(DirectionLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    
    return (light.color * diff +0.1)* diffuse_color;
}

void main()
{
	vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 result = CalcDirLight(directionLight, norm, viewDir);
    FragColor = vec4(result, 1.0);
}
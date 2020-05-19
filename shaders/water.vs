#version 330 core
layout (location = 0) in vec3 vertex;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float time;

#define NUM 4
uniform float A[NUM];
uniform vec2  D[NUM];
uniform float W[NUM];
uniform float P[NUM];

float Height(vec2 point)
{
    float h=0;
    for(int i=0;i<NUM;i++)
    {
        h+= A[i]*sin(dot(D[i],point)*W[i] + time*P[i]);
    }
    return h;
}

float DerivativeX(vec2 point)
{
    float d=0;
    for(int i=0;i<NUM;i++)
    {
        d+= W[i]*D[i].x*A[i]*cos(dot(D[i],point)*W[i] + time*P[i]);
    }
    return d;
}

float DerivativeZ(vec2 point)
{
    float d=0;
    for(int i=0;i<NUM;i++)
    {
        d+= W[i]*D[i].y*A[i]*cos(dot(D[i],point)*W[i] + time*P[i]);
    }
    return d;
}

void main()
{
    vec2 point = vec2(vertex.x,vertex.z);
    float h = Height(point);

    vec4 world_vert = model * vec4(vertex.x, h, vertex.z, 1.0);
    gl_Position = projection * view * world_vert;

    float dx = DerivativeX(point);
    float dz = DerivativeZ(point);
    Normal.x = -dx;
    Normal.y = 1;
    Normal.z = -dz;
    Normal = normalize(Normal);
    Normal = transpose(inverse(mat3(model))) * Normal;

    FragPos = world_vert.xyz;
}
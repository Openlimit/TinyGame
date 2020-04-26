#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec3 FragPos;

uniform vec3 color;

void main()
{    
    gPosition = FragPos;
    gAlbedoSpec.rgb = color;
}
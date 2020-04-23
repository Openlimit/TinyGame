#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>
layout (location = 1) in vec3 color; 
layout (location = 2) in mat4 model; 

out vec2 TexCoords;
out vec3 spriteColor;

uniform mat4 projection;

void main()
{
    TexCoords = vertex.zw;
    spriteColor = color;
    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}
#version 330 core
in vec2 TexCoords;
in vec3 spriteColor;

out vec4 color;

uniform sampler2D sprite;

void main()
{
    color = vec4(spriteColor, 1.0) * texture(sprite, TexCoords);
}
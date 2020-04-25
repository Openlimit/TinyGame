#version 330 core
out vec4 color;

in vec2 TexCoords;

uniform sampler2D scene_color;
uniform sampler2D scene_depth;

uniform float near_plane;
uniform float far_plane;

uniform vec3 fog_color;
uniform float fog_start;
uniform float fog_end;
uniform float fog_density;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));	
}

float LinearFog(float depth)
{
	float f = (fog_end - depth)/(fog_end - fog_start);
	f = clamp(f, 0, 1);
	return f;
}

float ExpFog(float depth)
{
	return exp(-fog_density*depth);
}

void main()
{
	float depthValue = texture(scene_depth, TexCoords).r;
	depthValue = LinearizeDepth(depthValue);
	//color = vec4(vec3(LinearizeDepth(depthValue) / far_plane), 1.0);
	//color = vec4(texture(scene_color, TexCoords).rgb, 1.0);

	//float f = ExpFog(depthValue);
	float f = LinearFog(depthValue);

	vec3 c = f * texture(scene_color, TexCoords).rgb + (1-f) * fog_color;
	color = vec4(c, 1.0);
}
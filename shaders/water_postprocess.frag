#version 330 core
out vec4 color;

in vec2 TexCoords;

uniform sampler2D scene_color;
uniform sampler2D gPosition;

uniform float near_plane;
uniform float far_plane;

uniform vec3 fog_color;
uniform float fog_start;
uniform float fog_end;
uniform float fog_density;

uniform vec3 light_color;
uniform vec3 light_pos;
uniform vec3 cam_pos;
uniform vec3 extinction;
uniform int step_num;
uniform float distance_ratio;

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

vec3 lightAt(vec3 pos)
{
    float lightDistance = length(light_pos - pos)*distance_ratio;
    vec3 transmittance = exp(-lightDistance * extinction);
    return light_color * transmittance;
}

vec3 scattering(vec3 ray, float near, float far)
{
    float step_size = (far-near)/step_num;
    vec3 transmittance = vec3(1);
    vec3 totalLight = vec3(0);
    float phase = 1/(4*3.1415);
	for(float distance = near; distance < far; distance += step_size)
    {
        vec3 pos = cam_pos + distance * ray;
        transmittance *= exp(-step_size * extinction);
        vec3 inScatterFactor = extinction;
        totalLight += transmittance * lightAt(pos) * inScatterFactor * phase * step_size;
	}
    return totalLight;
}

vec3 scattering_closed(vec3 ray, float rayDistance)
{
    vec3 q = cam_pos-light_pos;
    float b = dot(ray,q);
    float c = dot(q,q);
    float s = 1.0f/sqrt(c-b*b);

    float x = s*rayDistance;
    float y = s*b;
    return s*atan(x/(1+(x+y)*y));
}


void main()
{
	vec3 fragPos = texture(gPosition, TexCoords).rgb;
    vec3 view_path = fragPos - cam_pos;
    vec3 view_dir = normalize(view_path);
	float d = length(view_path)*distance_ratio;

	//float f = ExpFog(d);
	//float f = LinearFog(d);
	//vec3 c = f * texture(scene_color, TexCoords).rgb + (1-f) * fog_color;

    vec3 scatter_color = scattering(view_dir, 0, d);
    //vec3 scatter_color = scattering_closed(view_dir, d);
    vec3 surface_color = exp(-d * extinction) * texture(scene_color, TexCoords).rgb;

	color = vec4(scatter_color+surface_color, 1.0);
}
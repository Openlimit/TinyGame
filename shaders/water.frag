#version 330 core
out vec4 color;

in vec3 frag_pos;
in vec3 normal;

uniform vec3 light_pos;
uniform vec3 view_pos;
uniform vec3 diffuse_color;
uniform vec3 specular_color;

void main()
{
    vec3 ambient = 0.2*diffuse_color;

    vec3 light_dir = normalize(light_pos-frag_pos);
    float diffuse = max(dot(light_dir,normal),0);

    vec3 view_dir = normalize(view_pos-frag_pos);
    vec3 half_dir = normalize(view_dir+light_dir);
    float specular = pow(max(dot(normal,half_dir),0), 32);

    vec3 shading = ambient + diffuse_color*diffuse+specular_color*specular;
    color = vec4(shading, 1.0);
}
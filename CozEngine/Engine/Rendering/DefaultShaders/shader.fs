#version 330 core 
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

struct LMaterial
{
	vec3 Ambient;
	sampler2D Diffuse;
	sampler2D Specular;
	float Shininess;
};

struct LLight
{
	vec3 Position;
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
};

uniform LMaterial Material;
uniform LLight Light;
uniform vec3 ViewPos;

void main()
{ 
	vec3 Ambient = Light.Ambient * Material.Ambient;

	vec3 Norm = normalize(Normal);
	vec3 LightDir = normalize(Light.Position - FragPos);

	vec3 ViewDir = normalize(ViewPos - FragPos);
	vec3 ReflectDir = reflect(-LightDir, Norm);
	float Spec = pow(max(dot(ViewDir, ReflectDir), 0.0), Material.Shininess);

	vec3 Specular = Light.Specular * Spec * vec3(texture(Material.Specular, TexCoord));

	vec3 Diffuse = max(dot(Norm, LightDir), 0.0) * Light.Diffuse;

	vec3 Result = Ambient + Diffuse + Specular;
	FragColor = texture(Material.Diffuse, TexCoord) * vec4(Result, 1.0);
}
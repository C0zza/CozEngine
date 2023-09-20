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

struct LDirectionalLight
{
	vec3 Direction;
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
};

struct LPointLight
{
	vec3 Position;
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;

	float Constant;
	float Linear;
	float Quadratic;
};

uniform LMaterial Material;
uniform LPointLight Light;
uniform vec3 ViewPos;

void main()
{ 
	vec3 Ambient = Light.Ambient * Material.Ambient;

	vec3 Norm = normalize(Normal);
	
	vec3 LightDir = Light.Position - FragPos;
	float LightDistance = length(LightDir);
	LightDir = normalize(LightDir);

	float Attenuation = 1.0 / (Light.Constant + Light.Linear * LightDistance + Light.Quadratic * (LightDistance * LightDistance));

	vec3 ViewDir = normalize(ViewPos - FragPos);
	vec3 ReflectDir = reflect(-LightDir, Norm);
	float Spec = pow(max(dot(ViewDir, ReflectDir), 0.0), Material.Shininess);

	vec3 Specular = Light.Specular * Spec * vec3(texture(Material.Specular, TexCoord));

	vec3 Diffuse = max(dot(Norm, LightDir), 0.0) * Light.Diffuse;

	vec3 Result = (Ambient + Diffuse + Specular) * Attenuation;
	FragColor = texture(Material.Diffuse, TexCoord) * vec4(Result, 1.0);
}
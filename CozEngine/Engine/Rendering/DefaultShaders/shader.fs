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

struct LSpotLight
{
	vec3 Position;
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;

	vec3 Direction;
	float CutOff;
	float OuterCutOff;
};

uniform LMaterial Material;
uniform LSpotLight Light;
uniform vec3 ViewPos;

void main()
{ 
	vec3 Ambient = Light.Ambient * Material.Ambient;

	vec3 Norm = normalize(Normal);
	
	vec3 LightDir = Light.Position - FragPos;
	// float LightDistance = length(LightDir);
	LightDir = normalize(LightDir);

	// float Attenuation = 1.0 / (Light.Constant + Light.Linear * LightDistance + Light.Quadratic * (LightDistance * LightDistance));

	float Theta = dot(LightDir, normalize(-Light.Direction));
	float Epsilon = Light.CutOff - Light.OuterCutOff;
	float Intensity = clamp((Theta - Light.OuterCutOff) / Epsilon, 0.0, 1.0);

	vec3 ViewDir = normalize(ViewPos - FragPos);
	vec3 ReflectDir = reflect(-LightDir, Norm);
	float Spec = pow(max(dot(ViewDir, ReflectDir), 0.0), Material.Shininess);

	vec3 Specular = Light.Specular * Spec * vec3(texture(Material.Specular, TexCoord));

	vec3 Diffuse = max(dot(Norm, LightDir), 0.0) * Light.Diffuse;

	vec3 Result = (Ambient + Diffuse + Specular) * Intensity;
	FragColor = texture(Material.Diffuse, TexCoord) * vec4(Result, 1.0);
}
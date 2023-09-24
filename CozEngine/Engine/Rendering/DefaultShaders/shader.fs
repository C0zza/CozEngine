#version 330 core 
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

struct LMaterial
{
	// vec3 Ambient;
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

vec3 CalcDirectionalLight(LDirectionalLight Light, vec3 Normal, vec3 ViewDir);
vec3 CalcPointLight(LPointLight Light, vec3 Normal, vec3 FragPos, vec3 ViewDir);

uniform LDirectionalLight DirectionalLight;

// Should match number in PointLightComponent.h
#define MAX_NUM_POINT_LIGHT 4
uniform int ActivePointLights;
uniform LPointLight PointLights[MAX_NUM_POINT_LIGHT];

uniform LMaterial Material;
// uniform LSpotLight Light;
uniform vec3 ViewPos;

void main()
{ 
	vec3 Norm = normalize(Normal);
	vec3 ViewDir = normalize(ViewPos - FragPos);

	vec3 Result = CalcDirectionalLight(DirectionalLight, Norm, ViewDir);

	for(int i = 0; i < ActivePointLights; i++)
	{
		Result += CalcPointLight(PointLights[i], Norm, FragPos, ViewDir);
	}

	// Spot light?

	FragColor = vec4(Result, 1.0);

	//vec3 Ambient = Light.Ambient * Material.Ambient;

	//vec3 Norm = normalize(Normal);
	
	//vec3 LightDir = Light.Position - FragPos;
	// float LightDistance = length(LightDir);
	//LightDir = normalize(LightDir);

	// float Attenuation = 1.0 / (Light.Constant + Light.Linear * LightDistance + Light.Quadratic * (LightDistance * LightDistance));

	//float Theta = dot(LightDir, normalize(-Light.Direction));
	//float Epsilon = Light.CutOff - Light.OuterCutOff;
	//float Intensity = clamp((Theta - Light.OuterCutOff) / Epsilon, 0.0, 1.0);

	//vec3 ViewDir = normalize(ViewPos - FragPos);
	//vec3 ReflectDir = reflect(-LightDir, Norm);
	//float Spec = pow(max(dot(ViewDir, ReflectDir), 0.0), Material.Shininess);

	//vec3 Specular = Light.Specular * Spec * vec3(texture(Material.Specular, TexCoord));

	//vec3 Diffuse = max(dot(Norm, LightDir), 0.0) * Light.Diffuse;

	//vec3 Result = (Ambient + Diffuse + Specular) * Intensity;
	//FragColor = texture(Material.Diffuse, TexCoord) * vec4(Result, 1.0);
}

vec3 CalcDirectionalLight(LDirectionalLight Light, vec3 Normal, vec3 ViewDir)
{
	vec3 LightDir = normalize(-Light.Direction);

	float Diff = max(dot(Normal, LightDir), 0.0);

	vec3 ReflectDir = reflect(-LightDir, Normal);
	float Spec = pow(max(dot(ViewDir, ReflectDir), 0.0), Material.Shininess);

	vec3 Ambient = Light.Ambient * vec3(texture(Material.Diffuse, TexCoord));
	vec3 Diffuse = Light.Diffuse * Diff * vec3(texture(Material.Diffuse, TexCoord));
	vec3 Specular = Light.Specular * Spec * vec3(texture(Material.Specular, TexCoord));
	return Ambient + Diffuse + Specular;
}

vec3 CalcPointLight(LPointLight Light, vec3 Normal, vec3 FragPos, vec3 ViewDir)
{
	vec3 LightDir = normalize(Light.Position - FragPos);

	float Diff = max(dot(Normal, LightDir), 0.0);

	vec3 ReflectDir = reflect(-LightDir, Normal);
	float Spec = pow(max(dot(ViewDir, ReflectDir), 0.0), Material.Shininess);

	float Distance = length(Light.Position - FragPos);
	float Attenuation = 1.0 / (Light.Constant + Light.Linear * Distance + Light.Quadratic * (Distance * Distance));

	vec3 Ambient = Light.Ambient * vec3(texture(Material.Diffuse, TexCoord));
	vec3 Diffuse = Light.Diffuse * Diff * vec3(texture(Material.Diffuse, TexCoord));
	vec3 Specular = Light.Specular * Spec * vec3(texture(Material.Specular, TexCoord));

	return (Ambient + Diffuse + Specular) * Attenuation;
}
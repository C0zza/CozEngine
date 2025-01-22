#version 330 core 
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

struct LDirectionalLight
{
	bool IsActive;
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

	float Constant;
	float Linear;
	float Quadratic;
};

vec3 CalcDirectionalLight(LDirectionalLight Light, vec3 Normal, vec3 ViewDir, vec3 DiffuseSample, vec3 SpecularSample);
vec3 CalcPointLight(LPointLight Light, vec3 Normal, vec3 FragPos, vec3 ViewDir, vec3 DiffuseSample, vec3 SpecularSample);
vec3 CalcSpotLight(LSpotLight Light, vec3 Normal, vec3 FragPos, vec3 ViewDir, vec3 DiffuseSample, vec3 SpecularSample);

uniform LDirectionalLight DirectionalLight;

// Should match number in PointLightComponent.h
#define MAX_NUM_POINT_LIGHT 4
uniform int ActivePointLights;
uniform LPointLight PointLights[MAX_NUM_POINT_LIGHT];

// Should match number in SpotLightComponent.h
#define MAX_NUM_SPOT_LIGHT 4
uniform int ActiveSpotLights;
uniform LSpotLight SpotLights[MAX_NUM_SPOT_LIGHT];

layout (std140) uniform Matrices
{
    mat4 Projection;
    mat4 View;
	vec3 ViewPos;
};

uniform sampler2D GroundTexture;
uniform sampler2D WallTexture;

void main()
{ 
	vec3 Norm = normalize(Normal);
	vec3 ViewDir = normalize(ViewPos - FragPos);

	vec3 Result = vec3(0.0, 0.0, 0.0);

	vec4 GroundSample = texture(GroundTexture, TexCoord * 100);
	vec4 WallSample = texture(WallTexture, TexCoord * 60);

	float Angle = abs(dot(normalize(Normal), vec3(0, 1, 0)));

	float MinAngle = 0.4;
	float MaxAngle = 0.6;

	vec4 MixedSample = mix(WallSample, GroundSample, (Angle - MaxAngle) / (MaxAngle - MinAngle));
	vec3 DiffuseSample = vec3(MixedSample.r, MixedSample.g, MixedSample.b);	

	vec3 SpecularSample = vec3(0.0, 0.0, 0.0);

	if(DirectionalLight.IsActive)
	{
		Result += CalcDirectionalLight(DirectionalLight, Norm, ViewDir, DiffuseSample, SpecularSample);
	}

	for(int i = 0; i < ActivePointLights; i++)
	{
		Result += CalcPointLight(PointLights[i], Norm, FragPos, ViewDir, DiffuseSample, SpecularSample);
	}

	for(int i = 0; i < ActiveSpotLights; i++)
	{
		Result += CalcSpotLight(SpotLights[i], Norm, FragPos, ViewDir, DiffuseSample, SpecularSample);
	}

	FragColor = vec4(Result, 1.0);
}

vec3 CalcDirectionalLight(LDirectionalLight Light, vec3 Normal, vec3 ViewDir, vec3 DiffuseSample, vec3 SpecularSample)
{
	vec3 LightDir = normalize(-Light.Direction);

	float Diff = max(dot(Normal, LightDir), 0.0);

	//vec3 ReflectDir = reflect(-LightDir, Normal);
	//float Spec = pow(max(dot(ViewDir, ReflectDir), 0.0), Material.Shininess);

	vec3 Ambient = Light.Ambient * DiffuseSample;
	vec3 Diffuse = Light.Diffuse * Diff * DiffuseSample;
	//vec3 Specular = Light.Specular * Spec * SpecularSample;
	return Ambient + Diffuse;
}

vec3 CalcPointLight(LPointLight Light, vec3 Normal, vec3 FragPos, vec3 ViewDir, vec3 DiffuseSample, vec3 SpecularSample)
{
	vec3 LightDir = normalize(Light.Position - FragPos);

	float Diff = max(dot(Normal, LightDir), 0.0);

	//vec3 ReflectDir = reflect(-LightDir, Normal);
	//float Spec = pow(max(dot(ViewDir, ReflectDir), 0.0), Material.Shininess);

	float Distance = length(Light.Position - FragPos);
	float Attenuation = 1.0 / (Light.Constant + Light.Linear * Distance + Light.Quadratic * (Distance * Distance));

	vec3 Ambient = Light.Ambient * DiffuseSample;
	vec3 Diffuse = Light.Diffuse * Diff * DiffuseSample;
	//vec3 Specular = Light.Specular * Spec * SpecularSample;

	return (Ambient + Diffuse) * Attenuation;
}

vec3 CalcSpotLight(LSpotLight Light, vec3 Normal, vec3 FragPos, vec3 ViewDir, vec3 DiffuseSample, vec3 SpecularSample)
{
	vec3 LightDir = normalize(Light.Position - FragPos);

	float Theta = dot(LightDir, normalize(-Light.Direction));
	float Epsilon = Light.CutOff - Light.OuterCutOff;
	float Intensity = clamp((Theta - Light.OuterCutOff) / Epsilon, 0.0, 1.0);

	float Distance = length(Light.Position - FragPos);
	float Attenuation = 1.0 / (Light.Constant + Light.Linear * Distance + Light.Quadratic * (Distance * Distance));

	Intensity *= Attenuation;

	//vec3 ReflectDir = reflect(-LightDir, Normal);
	//float Spec = pow(max(dot(ViewDir, ReflectDir), 0.0), Material.Shininess);

	vec3 Ambient = Light.Ambient * DiffuseSample;
	vec3 Diffuse = Light.Diffuse * max(dot(Normal, LightDir), 0.0) * DiffuseSample;
	//vec3 Specular = Light.Specular * Spec * SpecularSample;

	// TODO: Add attenuation as well?
	return (Ambient + Diffuse) * Intensity;
}
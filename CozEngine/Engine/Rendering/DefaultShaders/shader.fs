#version 330 core 
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D DiffuseTexture1;
uniform sampler2D SpecularTexture1;
uniform vec3 LightColor;
uniform vec3 LightPos;
uniform vec3 ViewPos;

void main()
{ 
	float AmbientStrength = 0.2;
	vec3 Ambient = AmbientStrength * LightColor;

	vec3 Norm = normalize(Normal);
	vec3 LightDir = normalize(LightPos - FragPos);

	vec3 ViewDir = normalize(ViewPos - FragPos);
	vec3 ReflectDir = reflect(-LightDir, Norm);
	float Spec = pow(max(dot(ViewDir, ReflectDir), 0.0), 32);
	vec3 Specular = Spec * vec3(texture(SpecularTexture1, TexCoord));

	vec3 Diffuse = max(dot(Norm, LightDir), 0.0) * LightColor;

	vec3 Result = Ambient + Diffuse + Specular;
	FragColor = texture(DiffuseTexture1, TexCoord) * vec4(Result, 1.0);
}
#version 330 core 
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D DiffuseTexture1;
uniform vec3 LightColor;
uniform vec3 LightPos;

void main()
{ 
	float AmbientStrength = 0.2;
	vec3 Ambient = AmbientStrength * LightColor;

	vec3 Norm = normalize(Normal);
	vec3 LightDir = normalize(LightPos - FragPos);
	vec3 Diffuse = max(dot(Norm, LightDir), 0.0) * LightColor;

	FragColor = texture(DiffuseTexture1, TexCoord) * (vec4(Ambient, 1.0) + vec4(Diffuse, 1.0));
}
#version 330 core 
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;
out mat3 TBN;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat3 NormalMatrix;

void main() 
{ 
	vec3 T = normalize(vec3(Model * vec4(aTangent,   0.0)));
	vec3 B = normalize(vec3(Model * vec4(aBitangent, 0.0)));
	vec3 N = normalize(vec3(Model * vec4(aNormal,    0.0)));
	TBN = mat3(T, B, N);

	gl_Position = Projection * View * Model * vec4(aPos, 1.0);
	TexCoord = aTexCoord;
	FragPos = vec3(Model * vec4(aPos, 1.0));
	Normal = NormalMatrix * aNormal;
}
#version 330 core 
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

layout (std140) uniform Matrices
{
    mat4 Projection;
    mat4 View;
	vec3 ViewPos;
};

uniform mat4 Model;
uniform mat3 NormalMatrix;

uniform vec3 LandscapeSize;
uniform float HeightMapSize;

uniform sampler2D HeightMap;

void main() 
{ 
	float TexelSize = 1.0 / HeightMapSize;
	float Height = texture(HeightMap, aTexCoord).x;

	vec4 VertPos = vec4(aPos.x * LandscapeSize.x, Height * LandscapeSize.y, aPos.z * LandscapeSize.z, 1.0);
	// Center landscape mesh on model transform
	VertPos -= vec4(LandscapeSize.x / 2, LandscapeSize.y / 2, LandscapeSize.z / 2, 0);

	gl_Position = Projection * View * Model * VertPos;

	float t = texture(HeightMap, vec2(aTexCoord.x, aTexCoord.y - TexelSize), 0).r;
	float r = texture(HeightMap, vec2(aTexCoord.x + TexelSize, aTexCoord.y), 0).r;
	float l = texture(HeightMap, vec2(aTexCoord.x - TexelSize, aTexCoord.y), 0).r;
	float b = texture(HeightMap, vec2(aTexCoord.x, aTexCoord.y + TexelSize), 0).r;

	t *= LandscapeSize.y;
	b *= LandscapeSize.y;
	l *= LandscapeSize.y;
	r *= LandscapeSize.y;

	Normal = vec3((l - r) * 0.5, 1, (t - b) * 0.5);
	Normal = NormalMatrix * Normal;

	TexCoord = aTexCoord;
	FragPos = vec3(Model * VertPos);
}
#version 330 core 
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;
in mat3 TBN;

uniform int EntityID;

void main()
{ 
	FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
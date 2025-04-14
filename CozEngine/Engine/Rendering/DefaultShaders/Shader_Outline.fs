#version 330 core 
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;
in mat3 TBN;

void main()
{ 
	FragColor = vec4(0.98, 0.76, 0.12, 1.0);
}
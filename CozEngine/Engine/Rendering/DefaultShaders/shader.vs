#version 330 core 
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main() 
{ 
	gl_Position = Projection * View * Model * vec4(aPos, 1.0);
	TexCoord = aTexCoord;
	FragPos = vec3(Model * vec4(aPos, 1.0));
	// Costly calculation. See https://learnopengl.com/Lighting/Basic-Lighting - "One last thing".
	Normal = mat3(transpose(inverse(Model))) * aNormal;  
}
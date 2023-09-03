#version 330 core 
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D DiffuseTexture1;
uniform vec3 LightColor;

void main()
{ 
	FragColor = texture(DiffuseTexture1, TexCoord) * vec4(LightColor, 1.0);
}